#include "cppoam/MsgPackParser.h"

#include "BoxedValue.h"
#include "Instruction.h"
#include "OpCodes.h"
#include "Program.h"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace cppoam {

nullptr_t error(std::string msg) {
  std::cerr << msg << std::endl;
  return nullptr;
}

// TODO: Use actual #define unreachable
nullptr_t unreachable(std::string msg) {
  std::cerr << msg << std::endl;
  return nullptr;
}

MsgPackParser::MsgPackParser(msgpack::object_handle h)
  : MessageHandle_(std::move(h)) {}

MsgPackParser::MessageType_t MsgPackParser::parseMessageType() {
  const msgpack::object &obj = MessageHandle_.get();

  if (obj.type != msgpack::type::ARRAY || obj.via.array.size != 2) {
    error("Invalid format: expected array of size 2");
    return Invalid;
  }

  const msgpack::object &header = *obj.via.array.ptr;
  if (header.type != msgpack::type::POSITIVE_INTEGER) {
    error("Invalid format: expected int message type");
    return Invalid;
  }

  switch (header.as<int>()) {
    case 0: return ByteCode;
    case 1: return UnblockEvent;
    case 2: return Benchmark;
  }

  error("Invalid format: unrecognized message type");
  return Invalid;
}

ProgramRef MsgPackParser::parseProgram() {
  const msgpack::object &obj = MessageHandle_.get();

  if (obj.type != msgpack::type::ARRAY || obj.via.array.size != 2) {
    error("Invalid format: expected array of size 2");
    return ProgramRef(nullptr);
  }

  // Skip header (message type)
  const msgpack::object &body = *(obj.via.array.ptr + 1);

  try {
    // Return owning pointer to instance
    std::unique_ptr<Program> p = parseProgramBody(body);
    return ProgramRef(std::move(p));
  }
  catch (std::runtime_error ex) {
    error(std::string("Caught own exception: ") + ex.what());
  }
  catch (std::exception ex) {
    error(std::string("Caught 3rd-party exception: ") + ex.what());
  }
  catch (...) {
    error("Caught undefined thingy");
  }

  return ProgramRef(nullptr);
}

std::unique_ptr<Program> MsgPackParser::parseProgramBody(const msgpack::object &o) {
  if (o.type != msgpack::type::ARRAY || o.via.array.size != 2)
    throw std::runtime_error("Invalid format: expected array of size 2");

  const msgpack::object &section1 = *o.via.array.ptr;
  std::vector<std::string> ffi = parseSectionFFI(section1);

  const msgpack::object &section2 = *(o.via.array.ptr + 1);
  std::vector<Function> functions = parseSectionFunctions(section2);

  return std::make_unique<Program>(std::move(ffi), std::move(functions));
}

std::vector<std::string> MsgPackParser::parseSectionFFI(const msgpack::object &o) {
  if (o.type != msgpack::type::ARRAY)
    throw std::runtime_error("Invalid format: expected array of strings");

  msgpack::object *arr = o.via.array.ptr;
  size_t size = o.via.array.size;

  std::vector<std::string> ffi;
  ffi.reserve(size);

  for (int i = 0; i < size; i++) {
    if (arr->type != msgpack::type::STR)
      throw std::runtime_error("Invalid format: expected array of strings");

    ffi.push_back(arr[i].as<std::string>());
  }

  return ffi;
}

std::vector<Function> MsgPackParser::parseSectionFunctions(const msgpack::object &o) {
  if (o.type != msgpack::type::ARRAY)
    throw std::runtime_error("Invalid format: expected array of functions");

  msgpack::object *arr = o.via.array.ptr;
  size_t size = o.via.array.size;

  std::vector<Function> functions;
  functions.reserve(size);

  for (int i = 0; i < size; i++) {
    functions.push_back(parseFunction(arr[i]));
  }

  return functions;
}

struct MsgPackParser::FunctionSpec {
  unsigned Variables;
  unsigned Instructions;
};

Function MsgPackParser::parseFunction(const msgpack::object &o) {
  if (o.type != msgpack::type::ARRAY)
    throw std::runtime_error("Invalid format: expected array of instructions");

  unsigned headerSize;
  msgpack::object *header = o.via.array.ptr;

  FunctionSpec f = parseFunctionHeader(header, headerSize);
  return parseFunctionBody(std::move(f), header + headerSize);
};

MsgPackParser::FunctionSpec MsgPackParser::parseFunctionHeader(
    msgpack::object *arr, unsigned &itemsConsumed) {
  if (arr[0].type != msgpack::type::POSITIVE_INTEGER)
    throw std::runtime_error("Invalid format: expected number of locals");

  if (arr[1].type != msgpack::type::POSITIVE_INTEGER)
    throw std::runtime_error("Invalid format: expected number of instructions");

  unsigned variables = arr[0].as<unsigned>();
  unsigned instructions = arr[1].as<unsigned>();

  itemsConsumed = 2;
  return FunctionSpec{variables, instructions};
}

Function MsgPackParser::parseFunctionBody(FunctionSpec spec,
                                          msgpack::object *pos) {
  unsigned advance;
  std::vector<std::unique_ptr<Instruction>> instructions;

  for (int i = 0; i < spec.Instructions; i++) {
    instructions.push_back(parseInstruction(pos, advance));
    pos += advance;
  }

  return Function(spec.Variables, std::move(instructions));
}

std::unique_ptr<Instruction> MsgPackParser::parseInstruction(
    msgpack::object *pos, unsigned &advance) {
  if (pos[0].type != msgpack::type::POSITIVE_INTEGER)
    throw std::runtime_error("Invalid format: expected opcode");

  uint64_t opcode = pos[0].via.u64;
  advance = 1;

  switch (opcode) {
    case OpParallel: {
      advance += 2;
      auto left = pos[1].as<unsigned>();
      auto right = pos[2].as<unsigned>();
      return std::make_unique<ParallelInstruction>(left, right);
    }

    case OpPruning: {
      advance += 3;
      auto left = pos[1].as<unsigned>();
      auto index = pos[2].as<int>(); // -1 == none
      auto right = pos[3].as<unsigned>();
      return std::make_unique<PruningInstruction>(left, index, right);
    }

    case OpCall: {
      advance += 3;
      auto type = pos[1].as<unsigned>(); // 0 == TFun, 1 == TDynamic
      auto index = pos[2].as<int>(); // -1 == none ?
      auto args = parseCallArgs(pos[3]);
      return std::make_unique<CallInstruction>(type, index, args);
    }

    case OpTailCall: {
      advance += 3;
      auto type = pos[1].as<unsigned>(); // 0 == TFun, 1 == TDynamic
      auto index = pos[2].as<int>(); // -1 == none ?
      auto args = parseCallArgs(pos[3]);
      return std::make_unique<TailCallInstruction>(type, index, args);
    }

    case OpConst: {
      advance += 1;
      BoxedValue data = parseBoxedValue(pos[1]);
      return std::make_unique<ConstInstruction>(data);
    }

    case OpFFI: {
      advance += 2;
      auto index = pos[1].as<int>();
      auto args = parseCallArgs(pos[2]);
      return std::make_unique<FFICallInstruction>(index, args);
    }

    case OpOtherwise:
    case OpSequential:
    case OpCoeffect:
    case OpStop:
    case OpClosure:
    case OpLabel:
      throw std::runtime_error("Not yet implemented");
  }

  throw std::runtime_error("Invalid format: unrecognized opcode");
}

BoxedValue MsgPackParser::parseBoxedValue(const msgpack::object &o) {
  switch (o.type) {
    case msgpack::type::POSITIVE_INTEGER:
      return BoxedValue::createInt(o.via.u64);

    case msgpack::type::NEGATIVE_INTEGER:
      return BoxedValue::createInt(o.via.i64);

    default:
      throw std::runtime_error("Not yet implemented");
  }
}

std::vector<int> MsgPackParser::parseCallArgs(const msgpack::object &o) {
  if (o.type != msgpack::type::ARRAY)
    throw std::runtime_error("Invalid format: expected array of arguments");

  auto numArgs = o.via.array.size;
  msgpack::object *arr = o.via.array.ptr;

  std::vector<int> args;
  args.reserve(numArgs);

  for (int i = 0; i < numArgs; i++) {
    args.push_back(arr[i].as<int>());
  }

  return args;
}

}

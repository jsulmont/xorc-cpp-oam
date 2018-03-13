#pragma once

#include <memory>
#include <vector>

#include <msgpack.hpp>

#include "cppoam/ProgramRef.h"

namespace cppoam {

class BoxedValue;
class Program;
class Function;
class Instruction;

class MsgPackParser {
public:
  MsgPackParser(msgpack::object_handle h);

  enum MessageType_t {
    Invalid = -1,
    ByteCode = 0,
    UnblockEvent,
    Benchmark
  };

  MessageType_t parseMessageType();
  ProgramRef parseProgram();

private:
  msgpack::object_handle MessageHandle_;

  std::unique_ptr<Program> parseProgramBody(const msgpack::object &o);
  std::vector<std::string> parseSectionFFI(const msgpack::object &o);
  std::vector<Function> parseSectionFunctions(const msgpack::object &o);

  struct FunctionSpec;
  Function parseFunction(const msgpack::object &o);
  FunctionSpec parseFunctionHeader(msgpack::object *arr, unsigned &advance);
  Function parseFunctionBody(FunctionSpec spec, msgpack::object *body);

  std::unique_ptr<Instruction> parseInstruction(msgpack::object *pos, unsigned &advance);
  std::vector<int> parseCallArgs(const msgpack::object &o);
  BoxedValue parseBoxedValue(const msgpack::object &o);
};

}

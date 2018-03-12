#include "cppoam/MsgPackParser.h"
#include "Program.h"

#include <iostream>

namespace cppoam {

MsgPackParser::MsgPackParser(msgpack::object_handle h)
  : MessageHandle_(std::move(h)) {
  msgpack::object message = MessageHandle_.get();
  std::cerr << message << "\n";
}

MsgPackParser::MessageType_t MsgPackParser::parseMessageType() {
  return Invalid;
}

ProgramRef MsgPackParser::parseProgram() {
  return ProgramRef(nullptr);
}

}

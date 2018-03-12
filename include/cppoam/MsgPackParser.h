#pragma once

#include <msgpack.hpp>

#include "ProgramRef.h"

namespace cppoam {

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
};

}

#pragma once

#include <string>
#include <vector>

#include "Instruction.h"

namespace cppoam {

class Function {
  friend class FunctionRef;
  friend class MsgPackParser;

private:
  std::vector<Instruction> Instructions_;

};

class Program {
  friend class ProgramRef;

public:
  Program(std::vector<std::string> ffi, std::vector<Function> functions) {}

private:
  std::vector<std::string> FFI_;
  std::vector<Function> Functions_;

};

}

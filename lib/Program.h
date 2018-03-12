#pragma once

#include <string>
#include <vector>

namespace cppoam {

class Instruction {
};

class Function {
  friend class FunctionRef;

private:
  std::vector<Instruction> Instructions_;

};

class Program {
  friend class ProgramRef;

private:
  std::vector<std::string> FFI_;
  std::vector<Function> Functions_;

};

}

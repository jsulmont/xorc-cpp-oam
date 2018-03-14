#pragma once

#include <string>
#include <vector>

namespace cppoam {

class Program;
class Instruction;

class Function {
public:
  Function(unsigned numVariables,
           std::vector<std::unique_ptr<Instruction>> instructions);

  const Instruction &getFirstInstruction() const;

  unsigned getNumVariables() const {
    return NumVariables_;
  }

  static Function Invalid;

private:
  std::vector<std::unique_ptr<Instruction>> Instructions_;
  unsigned NumVariables_;
  Program *OwnerProgram_;

  // Private ctor for Invalid function
  Function() {
  }

  friend class Program;
  friend class Instruction;
  friend class FunctionRef;
  friend class MsgPackParser; // Really need that?
};

class Program {
public:
  Program(std::vector<std::string> ffi, std::vector<Function> functions);

  const Function &getMainFunction() const;
  const Instruction &getFirstInstruction() const;

  static Program Invalid;

private:
  std::vector<std::string> FFI_;
  std::vector<Function> Functions_;

  // Private ctor for Invalid program
  Program() {
  }

  friend class ProgramRef;
};
}

#include "Program.h"

#include "Instruction.h"

namespace cppoam {

Function Function::Invalid;

Program Program::Invalid;

Program::Program(std::vector<std::string> ffi, std::vector<Function> functions)
    : FFI_(std::move(ffi)), Functions_(std::move(functions)) {
  for (Function &f : Functions_) {
    f.OwnerProgram_ = this;
    for (std::unique_ptr<Instruction> &i : f.Instructions_) {
      i->setOwnerFunction(&f);
    }
  }
}

const Function &Program::getMainFunction() const {
  if (Functions_.empty())
    return Function::Invalid;

  return Functions_.back();
}

const Instruction &Program::getFirstInstruction() const {
  if (Functions_.empty())
    return Instruction::Invalid;

  return Functions_.front().getFirstInstruction();
}

const Instruction &Function::getFirstInstruction() const {
  if (Instructions_.empty())
    return Instruction::Invalid;

  return *Instructions_.back().get();
}

Function::Function(unsigned numVariables,
                   std::vector<std::unique_ptr<Instruction>> instructions)
    : Instructions_(std::move(instructions))
    , NumVariables_(numVariables) {}

}

#include "cppoam/ProgramRef.h"
#include "Program.h"

#include <memory>

namespace cppoam {

ProgramRef::ProgramRef(std::unique_ptr<Program> p)
  : Program_(std::move(p)) {}

size_t ProgramRef::getNumFFI() const {
  return Program_->FFI_.size();
}

size_t ProgramRef::getNumFunctions() const {
  return Program_->Functions_.size();
}

std::string ProgramRef::getFFI(unsigned idx) const {
  return Program_->FFI_.at(idx);
}

FunctionRef ProgramRef::getFunction(unsigned idx) const {
  return FunctionRef(Program_->Functions_.at(idx));
}

size_t FunctionRef::getNumInstructions() const {
  return Function_.Instructions_.size();
}

InstructionRef FunctionRef::getInstruction(unsigned idx) const {
  return InstructionRef(Function_.Instructions_.at(idx));
}

}

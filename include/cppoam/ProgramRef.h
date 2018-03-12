#pragma once

#include <memory>
#include <vector>

namespace cppoam {

class Program;
class Function;
class Instruction;

class InstructionRef {
public:
  InstructionRef(const Instruction &inst) : Instruction_(inst) {}

private:
  const Instruction &Instruction_;
};

class FunctionRef {
public:
  FunctionRef(const Function &func) : Function_(func) {}

  size_t getNumInstructions() const;
  InstructionRef getInstruction(unsigned idx) const;

private:
  const Function &Function_;
};

class ProgramRef {
public:
  ProgramRef(std::unique_ptr<Program> prog);

  size_t getNumFFI() const;
  std::string getFFI(unsigned idx) const;

  size_t getNumFunctions() const;
  FunctionRef getFunction(unsigned idx) const;

private:
  std::shared_ptr<Program> Program_;
};

}

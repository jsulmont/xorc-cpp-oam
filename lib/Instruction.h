#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "BoxedValue.h"

namespace cppoam {

class Function;
class NOPInstruction;

class Instruction {
public:
  void setOwnerFunction(Function *ownerFunction) {
    assert(OwnerFunction_ == nullptr);
    assert(ownerFunction != nullptr);
    OwnerFunction_ = ownerFunction;
  }

  static NOPInstruction Invalid;

private:
  Function *OwnerFunction_ = nullptr;
};

class ParallelInstruction : public Instruction {
public:
  ParallelInstruction(unsigned left, unsigned right) {
  }
};

class PruningInstruction : public Instruction {
public:
  PruningInstruction(unsigned left, int index, unsigned right) {
  }
};

class CallInstruction : public Instruction {
public:
  CallInstruction(unsigned type, int index, std::vector<int> args) {
  }
};

class TailCallInstruction : public Instruction {
public:
  TailCallInstruction(unsigned type, int index, std::vector<int> args) {
  }
};

class ConstInstruction : public Instruction {
public:
  ConstInstruction(BoxedValue val) {
  }
};

class FFICallInstruction : public Instruction {
public:
  FFICallInstruction(int index, std::vector<int> args) {
  }
};

class NOPInstruction : public Instruction {
public:
  NOPInstruction() = default;
};
}

#pragma once

namespace cppoam {

enum OpCodes {
  OpParallel = 0,
  OpOtherwise = 1,
  OpPruning = 2,
  OpSequential = 3,
  OpCall = 4,
  OpTailCall = 5,
  OpCoeffect = 6,
  OpStop = 7,
  OpConst = 8,
  OpClosure = 9,
  OpLabel = 10,
  OpFFI = 11
};

}

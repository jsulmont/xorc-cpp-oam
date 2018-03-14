#pragma once

#include <memory>

#include "cppoam/ProgramRef.h"
#include "cppoam/StateRef.h"

namespace cppoam {

class Interpreter {
public:
  StateRef run(ProgramRef program);

private:
  // ..

};

}

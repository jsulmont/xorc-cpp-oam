#pragma once

namespace cppoam {

class StateRef {
public:
  bool isDone();
  int getNumResults();
  int getIntResult(int idx);

private:
  // ..

};

}


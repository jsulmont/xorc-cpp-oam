#pragma once

#include <memory>

namespace cppoam {

class BoxedValue {
public:
  enum Types_t {
    Int
  };

  static BoxedValue createInt(int val);

private:
  struct Data_t {
    Types_t Type_;
    uint64_t Int_;
  };

  std::shared_ptr<Data_t> Data_;
};

}

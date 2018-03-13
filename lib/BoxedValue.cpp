#include "BoxedValue.h"

namespace cppoam {

BoxedValue BoxedValue::createInt(int val) {
  BoxedValue v;
  v.Data_ = std::make_shared<Data_t>();
  v.Data_->Type_ = Int;
  v.Data_->Int_ = val;
  return v;
}

}

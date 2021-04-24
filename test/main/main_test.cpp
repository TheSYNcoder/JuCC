#include "gtest/gtest.h"
#include "main/jucc.h"
using jucc::Hello;

TEST(juccmain, test1) {
  bool val = true;
  bool val2 = Hello() == "Hello World.\n";
  ASSERT_EQ(val, val2);
}

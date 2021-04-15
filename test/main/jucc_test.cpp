#include "main/jucc.h"

#include "gtest/gtest.h"

TEST(jucc, dummy) { ASSERT_TRUE(true); }

TEST(jucc, Radd) { ASSERT_EQ(100, jucc::Radd(0, 100)); }

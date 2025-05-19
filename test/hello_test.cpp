// test/hello_test.cpp
#include "core/hello.hpp"  // インクルードパスに載せた前提
#include <gtest/gtest.h>

TEST(HelloTest, ReturnsFortyTwo) { EXPECT_EQ(hello(), 42); }

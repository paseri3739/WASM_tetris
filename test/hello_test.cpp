// test/hello_test.cpp
#include <gtest/gtest.h>
#include <core/hello.hpp>  // インクルードパスに載せた前提

TEST(HelloTest, ReturnsFortyTwo) { EXPECT_EQ(hello(), 42); }

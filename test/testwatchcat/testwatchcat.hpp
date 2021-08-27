#include <gtest/gtest.h>

#include "timer.hpp"

class TimerTest : public ::testing::Test
{
 protected:
	watchcat::Timer * timer;

	void SetUp() override {
		timer = new watchcat::Timer();
	}

	void TearDown() override {
		delete timer;
	}
};
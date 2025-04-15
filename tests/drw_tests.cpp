#include "gtest/gtest.h"
#include "mocks/all_mock_class.hpp"
#include "mocks/X11/X.h"
#include "drw.h"

namespace {

TEST(Counter, Increment) {
	Display dpy;
	Window win;
	drw_create(&dpy, 0, win, 10, 10);
	EXPECT_EQ(0, 0);
}

}  // namespace

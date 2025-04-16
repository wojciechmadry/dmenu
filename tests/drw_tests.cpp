#include "gtest/gtest.h"
#include "mocks/all_mock_class.hpp"
#include "mocks/X11/X.h"
#include "mocks/X11/Xlib.h"
#include "drw.h"

namespace {

TEST(drw, drw_create) {
	Display dpy;
	dpy.screens = new Screen[1];
	Window win{1};
	const unsigned int width = 10;
	const unsigned int height = 12;
	const int screen = 0;
	auto result_drawable = 1;
	auto result_gc = 2;
	auto& xlib = getXlibMockResults();
	EXPECT_CALL(xlib, XCreatePixmap()).Times(testing::Exactly(1)).WillOnce(testing::Return(result_drawable));
	EXPECT_CALL(xlib, XCreateGC()).Times(testing::Exactly(1)).WillOnce(testing::Return(result_gc));
	EXPECT_CALL(xlib, XSetLineAttributes()).Times(testing::Exactly(1)).WillOnce(testing::Return(0));
	auto drw = drw_create(&dpy, screen, win, width, height);
	EXPECT_EQ(drw->dpy, &dpy);
	EXPECT_EQ(drw->screen, screen);
	EXPECT_EQ(drw->root, win);
	EXPECT_EQ(drw->w, width);
	EXPECT_EQ(drw->h, height);
	EXPECT_EQ(drw->drawable, result_drawable);
	EXPECT_EQ(drw->gc, result_gc);
	// Cleanup
	delete [] dpy.screens;
	free(drw);
}

}  // namespace

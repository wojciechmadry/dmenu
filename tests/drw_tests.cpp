#include "gtest/gtest.h"
#include "mocks/all_mock_class.hpp"
#include "mocks/X11/X.h"
#include "mocks/X11/Xlib.h"
#include "drw.h"

namespace {

class DrwTest : public testing::Test {
 protected:
	DrwTest() : xlib(getXlibMockResults()) {}

  void SetUp() override {
		dpy.screens = new Screen[1];
		dpy.screens[0] = Screen{};
		drw.dpy = &dpy;
  }

  void TearDown() override {
		delete [] dpy.screens;
  }

	Drw drw{};
	Display dpy{};
	Window win{};
	impl::XlibMockResults_& xlib;
};

TEST_F(DrwTest, drw_create) {
	win = 1;
	const unsigned int width = 10;
	const unsigned int height = 12;
	const int screen = 0;
	auto result_drawable = 1;
	auto result_gc = 2;
	EXPECT_CALL(xlib, XCreatePixmap()).Times(testing::Exactly(1)).WillOnce(testing::Return(result_drawable));
	EXPECT_CALL(xlib, XCreateGC()).Times(testing::Exactly(1)).WillOnce(testing::Return(result_gc));
	EXPECT_CALL(xlib, XSetLineAttributes()).Times(testing::Exactly(1)).WillOnce(testing::Return(0));
	auto drw_ptr = drw_create(&dpy, screen, win, width, height);
	EXPECT_EQ(drw_ptr->dpy, &dpy);
	EXPECT_EQ(drw_ptr->screen, screen);
	EXPECT_EQ(drw_ptr->root, win);
	EXPECT_EQ(drw_ptr->w, width);
	EXPECT_EQ(drw_ptr->h, height);
	EXPECT_EQ(drw_ptr->drawable, result_drawable);
	EXPECT_EQ(drw_ptr->gc, result_gc);
	free(drw_ptr);
}

TEST_F(DrwTest, drw_resize) {
	drw.w = 10;
	drw.h = 10;
	EXPECT_CALL(xlib, XCreatePixmap()).Times(testing::Exactly(2));
	EXPECT_CALL(xlib, XFreePixmap()).Times(testing::Exactly(1));
	drw_resize(&drw, 5, 8);
	EXPECT_EQ(drw.w, 5);
	EXPECT_EQ(drw.h, 8);
	drw.drawable = 1;
	drw_resize(&drw, 6, 7);
	EXPECT_EQ(drw.w, 6);
	EXPECT_EQ(drw.h, 7);
}

TEST_F(DrwTest, drw_free) {
	Drw* drw_ptr = (Drw*)malloc(sizeof(Drw));
	*drw_ptr = Drw{};

	auto f1 = (Fnt*)malloc(sizeof(Fnt));
	auto f2 = (Fnt*)malloc(sizeof(Fnt));
	auto f3 = (Fnt*)malloc(sizeof(Fnt));
	*f1 = Fnt{};
	*f2 = Fnt{};
	*f3 = Fnt{};
	f1->next = f2;
	f2->next = f3;
	f2->pattern = (FcPattern*) malloc(sizeof(FcPattern) * 1);
	f2->xfont = (XftFont*) malloc(sizeof(XftFont) * 1);
	drw_ptr->fonts = f1;
	EXPECT_CALL(xlib, XFreePixmap()).Times(testing::Exactly(1));
	EXPECT_CALL(xlib, XFreeGC()).Times(testing::Exactly(1));
	drw_free(drw_ptr);
}

}  // namespace

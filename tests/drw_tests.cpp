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

	template<typename T>
	T* create(const size_t elems = 1U) {
		T* res = (T*)malloc(sizeof(T) * elems);
		*res = T{};
		return res;
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
	Drw* drw_ptr = create<Drw>();

	auto f1 = create<Fnt>();
	auto f2 = create<Fnt>();
	auto f3 = create<Fnt>();
	f1->next = f2;
	f2->next = f3;
	f2->pattern = create<FcPattern>();
	f2->xfont = create<XftFont>();
	drw_ptr->fonts = f1;
	EXPECT_CALL(xlib, XFreePixmap()).Times(testing::Exactly(1));
	EXPECT_CALL(xlib, XFreeGC()).Times(testing::Exactly(1));
	drw_free(drw_ptr);
}

TEST_F(DrwTest, drw_fontset_create) {
	const char* fonts[] {"font1", "font2", "font3"};
	size_t fontcount = 3;
	EXPECT_CALL(xlib, XftFontOpenName()).Times(testing::Exactly(3)).WillRepeatedly(testing::Return(nullptr));
	auto fnt = drw_fontset_create(&drw, fonts, fontcount);
	EXPECT_EQ(fnt, nullptr);

	EXPECT_CALL(xlib, XftFontOpenName()).Times(testing::Exactly(3)).WillRepeatedly(testing::Invoke([&](){return create<XftFont>();}));
	EXPECT_CALL(xlib, FcNameParse()).Times(testing::Exactly(3)).WillRepeatedly(testing::Return(nullptr));
	fnt = drw_fontset_create(&drw, fonts, fontcount);
	EXPECT_EQ(fnt, nullptr);

	EXPECT_CALL(xlib, XftFontOpenName()).Times(testing::Exactly(3)).WillRepeatedly(testing::Invoke([&](){return create<XftFont>();}));
	EXPECT_CALL(xlib, FcNameParse()).Times(testing::Exactly(3)).WillRepeatedly(testing::Invoke([&](){return create<FcPattern>();}));
	fnt = drw_fontset_create(&drw, fonts, fontcount);
	ASSERT_NE(fnt, nullptr);
	EXPECT_EQ(fnt->xfont->ascent, 0);
	EXPECT_EQ(fnt->xfont->descent, 0);
	EXPECT_EQ(fnt->h, 0);
	EXPECT_EQ(fnt->dpy, drw.dpy);
	drw_fontset_free(drw.fonts);
}

TEST_F(DrwTest, drw_font_getexts) {
	EXPECT_CALL(xlib, XftTextExtentsUtf8()).Times(testing::Exactly(0));
	unsigned int len = 10;
	unsigned int w = 1;
	unsigned int h;
	drw_font_getexts(nullptr, nullptr, len, &w, &h);
	EXPECT_CALL(xlib, XftTextExtentsUtf8()).Times(testing::Exactly(1));
	auto font = create<Fnt>();
	font->h = 10;
	auto text = create<char>();
	drw_font_getexts(font, text, len, &w, &h);
	EXPECT_EQ(w, 0);
	EXPECT_EQ(h, font->h);
	free(font);
	free(text);
}

TEST_F(DrwTest, drw_scm_create) {
	EXPECT_CALL(xlib, XftColorAllocName()).Times(testing::Exactly(0));
	const char* clrnames[] = {"a", "b", "c"};
	EXPECT_EQ(drw_scm_create(&drw, clrnames, 1), nullptr);
	EXPECT_EQ(drw_scm_create(&drw, nullptr, 2), nullptr);
	EXPECT_EQ(drw_scm_create(nullptr, clrnames, 1), nullptr);

	EXPECT_CALL(xlib, XftColorAllocName()).Times(testing::Exactly(3)).WillRepeatedly(testing::Return(true));
	auto clr = drw_scm_create(&drw, clrnames, 3);
	EXPECT_NE(clr, nullptr);
	free(clr);
}

TEST_F(DrwTest, drw_cur) {
	int shape = 1;
	EXPECT_EQ(drw_cur_create(nullptr, shape), nullptr);
	EXPECT_CALL(xlib, XCreateFontCursor()).Times(testing::Exactly(1)).WillRepeatedly(testing::Return(Cursor{true}));
	auto cur = drw_cur_create(&drw, shape);
	EXPECT_TRUE(cur->cursor.set);
	EXPECT_NE(cur, nullptr);
	EXPECT_CALL(xlib, XFreeCursor()).Times(testing::Exactly(1));
	drw_cur_free(&drw, cur);
}

TEST_F(DrwTest, drw_setfontset) {
	Fnt fnt{};
	drw_setfontset(&drw, &fnt);
	EXPECT_EQ(drw.fonts, &fnt);
}

TEST_F(DrwTest, drw_setscheme) {
	Clr scm{};
	drw_setscheme(&drw, &scm);
	EXPECT_EQ(drw.scheme, &scm);
}

}  // namespace

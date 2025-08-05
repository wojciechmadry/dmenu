#include "dmenu_lib.h"
#include "mocks/X11/X.h"
#include "mocks/X11/Xlib.h"
#include "mocks/X11/extensions/Xinerama.h"
#include "mocks/all_mock_class.hpp"
#include "gtest/gtest.h"

// STD
#include <fstream>
#include <iostream>

namespace {
static struct item *create_item_list(int num_items) {
  if (num_items <= 0)
    return nullptr;

  std::vector<struct item> item_pool(num_items);
  for (int i = 0; i < num_items; ++i) {
    // Create a unique text for each item to make debugging easier
    char *text = new char[256];
    snprintf(text, 256, "item%d", i);
    item_pool[i].text = text;

    item_pool[i].left = (i > 0) ? &item_pool[i - 1] : nullptr;
    item_pool[i].right = (i < num_items - 1) ? &item_pool[i + 1] : nullptr;
  }

  // Since the vector will be destroyed, we need to copy the items to a more
  // stable memory location
  struct item *list_head = new struct item[num_items];
  for (int i = 0; i < num_items; ++i) {
    list_head[i] = item_pool[i];
    // Fix up the pointers to point to the new memory locations
    if (i > 0)
      list_head[i].left = &list_head[i - 1];
    if (i < num_items - 1)
      list_head[i].right = &list_head[i + 1];
  }
  list_head[0].left = nullptr;
  list_head[num_items - 1].right = nullptr;

  return list_head;
}
} // namespace

class DmenuTest : public testing::Test {
protected:
  void SetUp() override {
    // Setup a basic Drw and Fnt structure to avoid null pointers
    test_drw = (Drw *)malloc(sizeof(Drw));
    *test_drw = Drw{};
    test_font = (Fnt *)malloc(sizeof(Fnt));
    *test_font = Fnt{};
    test_drw->fonts = test_font;
    test_items = create_item_list(10);
    curr = test_items;
    prev = test_items;

    // Set the global variables used by the function under test
    drw = test_drw;
    lrpad = 10; // Set a known padding value
  }

  void TearDown() override {
    for (int i = 0; i < 10; ++i) {
      delete[] test_items[i].text;
    }
    delete[] test_items;
    curr = nullptr;
    prev = nullptr;
    free(test_font);
    free(test_drw);
  }

  Drw *test_drw;
  Fnt *test_font;
  impl::XlibMockResults_ &xlib_mock = getXlibMockResults();
  struct item *test_items;
};

TEST(dmenuTest, macros) {
  XineramaScreenInfo xsi{};
  xsi.x_org = 5;
  xsi.y_org = 4;
  xsi.width = 3;
  xsi.height = 6;
  EXPECT_EQ(INTERSECT(-1, -1, 1, 1, xsi), 0);
}

TEST_F(DmenuTest, TextwClamp_WidthIsClamped) {
  const char *test_str = "some long text";
  unsigned int n = 100;
  EXPECT_CALL(xlib_mock, XftTextExtentsUtf8(testing::_, testing::_, testing::_,
                                            testing::_, testing::_))
      .WillRepeatedly(
          testing::Invoke([](Display *, XftFont *, const FcChar8 *, int,
                             XGlyphInfo *extents) { extents->xOff = 95; }));

  const auto result = textw_clamp(test_str, n);
  EXPECT_EQ(result, 100);
}

TEST_F(DmenuTest, TextwClamp_WidthIsNotClamped) {
  const char *test_str = "short string";
  unsigned int n = 100;

  EXPECT_CALL(xlib_mock, XftTextExtentsUtf8(testing::_, testing::_, testing::_,
                                            testing::_, testing::_))
      .WillRepeatedly(
          testing::Invoke([](Display *, XftFont *, const FcChar8 *, int,
                             XGlyphInfo *extents) { extents->xOff = 50; }));

  const auto result = textw_clamp(test_str, n);
  EXPECT_EQ(result, 100);
}

TEST_F(DmenuTest, TextwClamp_FullWidthExceedsClamp) {
  const char *test_str = "Very very long string long long long";
  unsigned int n = 100; // The clamp value

  EXPECT_CALL(xlib_mock, XftTextExtentsUtf8(testing::_, testing::_, testing::_,
                                            testing::_, testing::_))
      .WillRepeatedly(
          testing::Invoke([](Display *, XftFont *, const FcChar8 *, int,
                             XGlyphInfo *extents) { extents->xOff = 150; }));

  const auto result = textw_clamp(test_str, n);
  EXPECT_EQ(result, 100);
}

TEST_F(DmenuTest, AppendItem_ToEmptyList) {
  struct item *list = NULL;
  struct item *last = NULL;
  struct item new_item;

  appenditem(&new_item, &list, &last);

  EXPECT_EQ(list, &new_item);
  EXPECT_EQ(last, &new_item);

  EXPECT_EQ(new_item.left, (struct item *)NULL);
  EXPECT_EQ(new_item.right, (struct item *)NULL);
}

TEST_F(DmenuTest, AppendItem_ToNonEmptyList) {
  char *f = new char[]{"first"};
  struct item first_item = {.text = f, .left = NULL, .right = NULL, .out = 0};
  struct item *list = &first_item;
  struct item *last = &first_item;
  struct item new_item;

  appenditem(&new_item, &list, &last);

  // The list should still start with the first item.
  EXPECT_EQ(list, &first_item);

  // The last item should now be the new item.
  EXPECT_EQ(last, &new_item);

  // Check the pointers to make sure the list is correctly linked.
  EXPECT_EQ(first_item.right, &new_item);
  EXPECT_EQ(new_item.left, &first_item);
  EXPECT_EQ(new_item.right, (struct item *)NULL);
  delete[] f;
}

TEST_F(DmenuTest, AppendItem_MultipleItems) {
  struct item *list = NULL;
  struct item *last = NULL;

  struct item item1, item2, item3;

  // Append the first item
  appenditem(&item1, &list, &last);
  EXPECT_EQ(list, &item1);
  EXPECT_EQ(last, &item1);
  EXPECT_EQ(item1.left, (struct item *)NULL);
  EXPECT_EQ(item1.right, (struct item *)NULL);

  // Append the second item
  appenditem(&item2, &list, &last);
  EXPECT_EQ(list, &item1);
  EXPECT_EQ(last, &item2);
  EXPECT_EQ(item1.right, &item2);
  EXPECT_EQ(item2.left, &item1);
  EXPECT_EQ(item2.right, (struct item *)NULL);

  // Append the third item
  appenditem(&item3, &list, &last);
  EXPECT_EQ(list, &item1);
  EXPECT_EQ(last, &item3);
  EXPECT_EQ(item2.right, &item3);
  EXPECT_EQ(item3.left, &item2);
  EXPECT_EQ(item3.right, (struct item *)NULL);
}

TEST_F(DmenuTest, VerticalList_Forward) {
  lines = 3;
  bh = 10;

  calcoffsets();

  EXPECT_EQ(next, &test_items[3]);
  EXPECT_EQ(prev, &test_items[0]);
}

TEST_F(DmenuTest, VerticalList_Paginate) {
  lines = 3;
  bh = 10;
  curr = &test_items[3];

  calcoffsets();

  EXPECT_EQ(next, &test_items[6]);
  EXPECT_EQ(prev, &test_items[0]);
}

TEST_F(DmenuTest, HorizontalList_Forward) {
  lines = 0;
  bh = 10;
  mw = 500;
  promptw = 50;
  inputw = 100;
  lrpad = 10;

  EXPECT_CALL(xlib_mock, XftTextExtentsUtf8(testing::_, testing::_, testing::_,
                                            testing::_, testing::_))
      .WillRepeatedly(testing::Invoke([](Display *, XftFont *,
                                         const FcChar8 *string, int,
                                         XGlyphInfo *extents) {
        if (strcmp((const char *)string, "<") == 0 ||
            strcmp((const char *)string, ">") == 0) {
          extents->xOff = 0; // TEXTW adds lrpad, so this gives a width of 10
        } else {
          extents->xOff =
              90; // textw_clamp adds lrpad, so this gives a width of 100
        }
      }));

  calcoffsets();

  EXPECT_EQ(next, &test_items[1]);
  EXPECT_EQ(prev, &test_items[0]);
}

TEST_F(DmenuTest, HorizontalList_Paginate) {
  curr = &test_items[3];

  EXPECT_CALL(xlib_mock, XftTextExtentsUtf8(testing::_, testing::_, testing::_,
                                            testing::_, testing::_))
      .WillRepeatedly(
          testing::Invoke([](Display *, XftFont *, const FcChar8 *string, int,
                             XGlyphInfo *extents) {
            if (strcmp((const char *)string, "<") == 0 ||
                strcmp((const char *)string, ">") == 0) {
              extents->xOff = 0;
            } else {
              extents->xOff = 90;
            }
          }));

  calcoffsets();

  EXPECT_EQ(next, &test_items[3]);
  EXPECT_EQ(prev, &test_items[3]);
}

TEST_F(DmenuTest, AllItemsFit) {
  lines = 10; // We have 10 items, so they all fit
  bh = 10;

  calcoffsets();

  EXPECT_EQ(next, (struct item *)NULL);
  EXPECT_EQ(prev, &test_items[0]);
}

TEST_F(DmenuTest, DrawsSelectedItem) {
  char *text = new char[256];
  snprintf(text, 256, "selected item");

  struct item test_item = {.text = text, .out = 0};
  sel = &test_item; // Mark this item as the selected one

  int result = drawitem(&test_item, 10, 20, 100);

  EXPECT_EQ(drw->scheme, scheme[SchemeSel]);

  EXPECT_EQ(result, 0);
  delete[] text;
  sel = nullptr;
}

TEST_F(DmenuTest, DrawsOutItem) {
  char *text = new char[256];
  snprintf(text, 256, "out item");

  struct item test_item = {.text = text, .out = 1};

  int result = drawitem(&test_item, 10, 20, 100);

  EXPECT_EQ(drw->scheme, scheme[SchemeSel]);

  EXPECT_EQ(result, 0);
  delete[] text;
}

TEST_F(DmenuTest, DrawsNormalItem) {
  char *text = new char[256];
  snprintf(text, 256, "norm item");

  struct item test_item = {.text = text, .out = 0};

  int result = drawitem(&test_item, 10, 20, 100);

  EXPECT_EQ(drw->scheme, scheme[SchemeSel]);

  EXPECT_EQ(result, 0);
  delete[] text;
}

TEST(DmenuTestSimple, Clean) {
  // Set the global variables used by the function under test
  drw = (Drw *)malloc(sizeof(Drw));
  *drw = Drw{};
  drw->fonts = (Fnt *)malloc(sizeof(Fnt));
  *drw->fonts = Fnt{};
  popcache = (char *)malloc(sizeof(char));

  items = (item *)malloc(sizeof(item));
  *items = item{};
  items->text = (char *)malloc(1);
  popitems = (item *)malloc(sizeof(item));
  *popitems = item{};
  popitems->text = (char *)malloc(1);

  impl::XlibMockResults_ &xlib_mock = getXlibMockResults();
  EXPECT_CALL(xlib_mock, XFreePixmap()).Times(1);
  EXPECT_CALL(xlib_mock, XSync()).Times(1);
  EXPECT_CALL(xlib_mock, XFreeGC()).Times(1);
  ASSERT_NO_FATAL_FAILURE(cleanup());
}

TEST(CIStrStrTest, Found) {
  const char *haystack = "Hello World";
  const char *needle = "world";
  EXPECT_STREQ(cistrstr(haystack, needle), "World");
}

TEST(CIStrStrTest, NotFound) {
  const char *haystack = "Hello World";
  const char *needle = "goodbye";
  EXPECT_EQ(cistrstr(haystack, needle), nullptr);
}

TEST(CIStrStrTest, CaseMismatch) {
  const char *haystack = "Hello World";
  const char *needle = "WoRlD";
  EXPECT_STREQ(cistrstr(haystack, needle), "World");
}

TEST(CIStrStrTest, EmptyNeedle) {
  const char *haystack = "Hello World";
  const char *needle = "";
  EXPECT_STREQ(cistrstr(haystack, needle), haystack);
}

TEST(CIStrStrTest, EmptyHaystack) {
  const char *haystack = "";
  const char *needle = "world";
  EXPECT_EQ(cistrstr(haystack, needle), nullptr);
}

TEST(CIStrStrTest, BothEmpty) {
  const char *haystack = "";
  const char *needle = "";
  EXPECT_STREQ(cistrstr(haystack, needle), haystack);
}

TEST(CIStrStrTest, ExactMatch) {
  const char *haystack = "Hello";
  const char *needle = "Hello";
  EXPECT_STREQ(cistrstr(haystack, needle), haystack);
}

TEST(CIStrStrTest, SubstringAtStart) {
  const char *haystack = "Hello World";
  const char *needle = "hELLo";
  EXPECT_STREQ(cistrstr(haystack, needle), "Hello World");
}

TEST(CIStrStrTest, SubstringAtEnd) {
  const char *haystack = "Hello World";
  const char *needle = "world";
  EXPECT_STREQ(cistrstr(haystack, needle), "World");
}

TEST(CIStrStrTest, PartialMatch) {
  const char *haystack = "ababcd";
  const char *needle = "abc";
  EXPECT_STREQ(cistrstr(haystack, needle), "abcd");
}

TEST(CIStrStrTest, MultipleMatches) {
  // cistrstr should find the first occurrence
  const char *haystack = "Hello hello Hello";
  const char *needle = "hello";
  EXPECT_STREQ(cistrstr(haystack, needle), "Hello hello Hello");
}

// Test fixture for the drawmenu function
class DrawMenuTest : public DmenuTest {
protected:
  struct item *test_items;
  Display dpy_mock;

  void SetUp() override {
    DmenuTest::SetUp();

    // --- Setup the global state that drawmenu() uses ---
    mw = 1000;
    mh = 200;
    bh = 20;
    lrpad = 10;
    promptw = 50;
    inputw = 200;
    cursor = 0;
    strcpy(text, "test");

    // Mock display and schemes
    dpy = &dpy_mock;
    drw->dpy = dpy;
    drw->dpy->screens = new Screen[10];
    drw->fonts = new Fnt{};
    drw->fonts->xfont = new XftFont{};
    scheme[SchemeNorm] = (Clr *)ecalloc(1, sizeof(Clr));
    scheme[SchemeSel] = (Clr *)ecalloc(1, sizeof(Clr));

    // Create a list of items
    test_items = create_item_list(5);
    matches = test_items;
    curr = test_items;
    next = &test_items[3]; // Represents the end of the current page

    // Default to no prompt
    prompt = NULL;

    // Mock TEXTW macro by controlling the underlying function
    EXPECT_CALL(xlib_mock, XftTextExtentsUtf8)
        .WillRepeatedly(
            testing::Invoke([](Display *, XftFont *, const FcChar8 *string,
                               int len, XGlyphInfo *extents) {
              extents->xOff =
                  len * 10; // Simple width calculation: 10 pixels per character
            }));
  }

  void TearDown() override {
    free(scheme[SchemeNorm]);
    free(scheme[SchemeSel]);
    for (int i = 0; i < 5; ++i)
      delete[] test_items[i].text;
    delete[] test_items;
    delete[] dpy_mock.screens;
    delete[] drw->fonts->xfont;
    delete[] drw->fonts;
    DmenuTest::TearDown();
  }
};

TEST_F(DrawMenuTest, DrawsWithPrompt) {
  // Verifies that if a prompt is set, it is drawn correctly.
  prompt = "Prompt:";
  promptw = TEXTW(prompt);

  EXPECT_CALL(xlib_mock, XFillRectangle).Times(testing::AtLeast(1));
  EXPECT_CALL(xlib_mock, XSetForeground).Times(testing::AtLeast(2));
  EXPECT_CALL(xlib_mock, XftDrawStringUtf8).Times(testing::AtLeast(3));
  EXPECT_CALL(xlib_mock, XCopyArea).Times(1);

  drawmenu();
}

TEST_F(DrawMenuTest, DrawsVerticalList) {
  lines = 3;
  next = &test_items[3];
  EXPECT_CALL(xlib_mock, XFillRectangle).Times(testing::AtLeast(2));
  EXPECT_CALL(xlib_mock, XSetForeground).Times(testing::AtLeast(2));
  EXPECT_CALL(xlib_mock, XftDrawStringUtf8).Times(4);
  EXPECT_CALL(xlib_mock, XCopyArea).Times(1);

  drawmenu();
}

TEST_F(DrawMenuTest, DrawsHorizontalList) {
  lines = 0;
  curr = &test_items[1];
  curr->left = &test_items[0];
  next = &test_items[4];

  EXPECT_CALL(xlib_mock, XFillRectangle).Times(testing::AtLeast(2));
  EXPECT_CALL(xlib_mock, XftDrawStringUtf8).Times(6);
  EXPECT_CALL(xlib_mock, XCopyArea).Times(1);
  EXPECT_CALL(xlib_mock, XSync).Times(1);

  drawmenu();
}

TEST_F(DrawMenuTest, DrawsCursor) {
  strcpy(text, "test");
  cursor = 2;
  lines = 0;
  matches = NULL;

  EXPECT_CALL(xlib_mock, XFillRectangle).Times(testing::AtLeast(2));
  EXPECT_CALL(xlib_mock, XftDrawStringUtf8).Times(1);
  EXPECT_CALL(xlib_mock, XCopyArea).Times(1);

  drawmenu();
}

TEST_F(DrawMenuTest, DrawsWhenNoMatches) {
  matches = NULL;
  lines = 0;

  EXPECT_CALL(xlib_mock, XFillRectangle).Times(testing::AtLeast(2));
  EXPECT_CALL(xlib_mock, XftDrawStringUtf8).Times(1);
  EXPECT_CALL(xlib_mock, XCopyArea).Times(1);
  EXPECT_CALL(xlib_mock, XSync).Times(1);

  drawmenu();
}

// Test fixture for the grabfocus function
class GrabFocusTest : public DmenuTest {
protected:
  Display dpy_mock;

  void SetUp() override {
    DmenuTest::SetUp();
    dpy = &dpy_mock;
    win = 12345; // A dummy window ID for our target window
  }
};

TEST_F(GrabFocusTest, FocusIsAlreadyGrabbed) {
  EXPECT_CALL(xlib_mock, XGetInputFocus(dpy, testing::_, testing::_))
      .WillOnce(testing::Invoke([](Display *, Window *focus_return, int *) {
        *focus_return = win; // Simulate that 'win' already has focus
      }));

  EXPECT_CALL(xlib_mock,
              XSetInputFocus(testing::_, testing::_, testing::_, testing::_))
      .Times(0);

  EXPECT_NO_THROW(grabfocus());
}

TEST_F(GrabFocusTest, GrabsFocusAfterOneAttempt) {
  EXPECT_CALL(xlib_mock, XGetInputFocus(dpy, testing::_, testing::_))
      .WillOnce(testing::Invoke([](Display *, Window *focus_return, int *) {
        *focus_return = 0; // First, report focus is elsewhere
      }))
      .WillOnce(testing::Invoke([](Display *, Window *focus_return, int *) {
        *focus_return = win; // Then, report success
      }));

  EXPECT_CALL(xlib_mock, XSetInputFocus(dpy, win, RevertToParent, CurrentTime))
      .Times(1);

  EXPECT_NO_THROW(grabfocus());
}

TEST_F(GrabFocusTest, FailsToGrabFocusAndDies) {
  EXPECT_CALL(xlib_mock, XGetInputFocus(dpy, testing::_, testing::_))
      .WillRepeatedly(
          testing::Invoke([](Display *, Window *focus_return, int *) {
            *focus_return = 0; // Always report focus is not on 'win'
          }));

  EXPECT_CALL(xlib_mock, XSetInputFocus(dpy, win, RevertToParent, CurrentTime))
      .Times(100);

  EXPECT_DEATH(grabfocus(), "cannot grab focus");
}

class GrabKeyboardTest : public DmenuTest {
protected:
  Display dpy_mock;

  void SetUp() override {
    DmenuTest::SetUp();
    dpy = &dpy_mock;
    embed = NULL; // Default to not being embedded
  }
};

TEST_F(GrabKeyboardTest, ReturnsImmediatelyIfEmbedded) {
  embed = (char *)"some_window_id"; // Set embed to a non-NULL value

  EXPECT_CALL(xlib_mock, XGrabKeyboard(testing::_, testing::_, testing::_,
                                       testing::_, testing::_, testing::_))
      .Times(0);

  grabkeyboard();
}

TEST_F(GrabKeyboardTest, GrabsKeyboardOnFirstTry) {
  EXPECT_CALL(xlib_mock, XGrabKeyboard(dpy, testing::_, True, GrabModeAsync,
                                       GrabModeAsync, CurrentTime))
      .WillOnce(testing::Return(GrabSuccess));

  EXPECT_NO_THROW(grabkeyboard());
}

TEST_F(GrabKeyboardTest, GrabsKeyboardAfterMultipleTries) {
  EXPECT_CALL(xlib_mock, XGrabKeyboard(dpy, testing::_, True, GrabModeAsync,
                                       GrabModeAsync, CurrentTime))
      .WillOnce(testing::Return(1))            // Fail
      .WillOnce(testing::Return(1))            // Fail
      .WillOnce(testing::Return(GrabSuccess)); // Succeed

  EXPECT_NO_THROW(grabkeyboard());
}

TEST_F(GrabKeyboardTest, FailsToGrabKeyboardAndDies) {
  EXPECT_CALL(xlib_mock, XGrabKeyboard(dpy, testing::_, True, GrabModeAsync,
                                       GrabModeAsync, CurrentTime))
      .WillRepeatedly(testing::Return(1)); // Always return a failure code

  EXPECT_DEATH(grabkeyboard(), "cannot grab keyboard");
}

class SortItemsByPopTest : public DmenuTest {
protected:
  struct item *test_items;
  struct item *popular_items;

  void SetUp() override {
    DmenuTest::SetUp();

    // --- Setup the global state for the tests ---

    // Create a standard list of items to be sorted
    test_items = (struct item *)ecalloc(5, sizeof(struct item));
    test_items[0] = {.text = (char *)"apple"};
    test_items[1] = {.text = (char *)"banana"};
    test_items[2] = {.text = (char *)"cherry"};
    test_items[3] = {.text = (char *)"date"};
    test_items[4] = {.text = NULL}; // End of list

    // Create a list of popular items
    popular_items = (struct item *)ecalloc(4, sizeof(struct item));
    popular_items[0] = {.text = (char *)"cherry"}; // Most popular
    popular_items[1] = {.text = (char *)"apple"};  // Second most
    popular_items[2] = {.text = (char *)"other"};  // Not in the list
    popular_items[3] = {.text = NULL};

    // Set the global popitems pointer
    popitems = popular_items;
  }

  void TearDown() override {
    free(test_items);
    free(popular_items);
    DmenuTest::TearDown();
  }
};

TEST_F(SortItemsByPopTest, SortsBasedOnPopularity) {
  sortitemsbypop(test_items, NULL);

  EXPECT_STREQ(test_items[0].text, "cherry");
  EXPECT_STREQ(test_items[1].text, "apple");
  EXPECT_STREQ(test_items[2].text, "banana");
  EXPECT_STREQ(test_items[3].text, "date");
}

TEST_F(SortItemsByPopTest, NoMatches) {
  popitems[0].text = (char *)"x"; // Change popitems to not match anything
  popitems[1].text = (char *)"y";

  sortitemsbypop(test_items, NULL);

  EXPECT_STREQ(test_items[0].text, "apple");
  EXPECT_STREQ(test_items[1].text, "banana");
  EXPECT_STREQ(test_items[2].text, "cherry");
  EXPECT_STREQ(test_items[3].text, "date");
}

TEST_F(SortItemsByPopTest, SortsPartialList) {
  sortitemsbypop(test_items, &test_items[2]);

  EXPECT_STREQ(test_items[0].text, "cherry");
  EXPECT_STREQ(test_items[1].text, "apple");
  EXPECT_STREQ(test_items[2].text, "banana");
  EXPECT_STREQ(test_items[3].text, "date"); // Unchanged
}

TEST_F(SortItemsByPopTest, EmptyPopItems) {
  popitems = NULL;

  sortitemsbypop(test_items, NULL);

  // Order should remain the same
  EXPECT_STREQ(test_items[0].text, "apple");
  EXPECT_STREQ(test_items[1].text, "banana");
  EXPECT_STREQ(test_items[2].text, "cherry");
}

TEST_F(SortItemsByPopTest, NullInputList) {
  ASSERT_NO_FATAL_FAILURE(sortitemsbypop(NULL, NULL));
}

class IncPopTest : public DmenuTest {
protected:
  struct item *popular_items;
  const char *temp_cache_file = "dmenu_pop_test.txt";

  void SetUp() override {
    DmenuTest::SetUp();

    popular_items = (struct item *)ecalloc(3, sizeof(struct item));
    popular_items[0] = {.text = (char *)"apple", .out = 10};
    popular_items[1] = {.text = (char *)"banana", .out = 5};
    popular_items[2] = {.text = NULL};

    popitems = popular_items;
    popcache = (char *)temp_cache_file;
  }

  void TearDown() override {
    free(popular_items);
    remove(temp_cache_file); // Clean up the temporary file
    DmenuTest::TearDown();
  }

  std::string read_cache_file() {
    std::ifstream file(temp_cache_file);
    if (!file.is_open())
      return "";
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
  }
};

TEST_F(IncPopTest, IncrementsExistingPopularItem) {
  struct item selected_item = {.text = (char *)"apple"};

  incpop(&selected_item);

  EXPECT_EQ(popitems[0].out, 11);

  std::string file_content = read_cache_file();
  std::string expected_content = "apple 11\nbanana 5\n";
  EXPECT_EQ(file_content, expected_content);
}

TEST_F(IncPopTest, AddsNewPopularItem) {
  struct item selected_item = {.text = (char *)"cherry"};

  incpop(&selected_item);

  std::string file_content = read_cache_file();
  std::string expected_content = "apple 10\nbanana 5\ncherry 1\n";
  EXPECT_EQ(file_content, expected_content);
}

TEST_F(IncPopTest, HandlesNullSelection) {
  incpop(NULL);

  std::string file_content = read_cache_file();
  EXPECT_TRUE(file_content.empty());
}

TEST_F(IncPopTest, ClampsPopularityAt999) {
  popitems[0].out = 999;
  struct item selected_item = {.text = (char *)"apple"};

  incpop(&selected_item);

  EXPECT_EQ(popitems[0].out, 1000);

  std::string file_content = read_cache_file();
  std::string expected_content = "apple 999\nbanana 5\n";
  EXPECT_EQ(file_content, expected_content);
}

TEST_F(IncPopTest, HandlesFileOpenFailure) {
  popcache = (char *)"/non_existent_dir/test.txt";
  struct item selected_item = {.text = (char *)"apple"};

  ASSERT_NO_FATAL_FAILURE(incpop(&selected_item));
}

class MatchTest : public DmenuTest {
protected:
  struct item *test_items;
  struct item *popular_items;
  Display dpy_mock;

  void SetUp() override {
    DmenuTest::SetUp();

    // --- Setup the global state for the tests ---
    fstrncmp = strncmp; // Use case-sensitive matching by default
    fstrstr = strstr;

    // Create a standard list of items
    test_items = (struct item *)ecalloc(6, sizeof(struct item));
    test_items[0] = {.text = (char *)"apple pie"};
    test_items[1] = {.text = (char *)"apple"};
    test_items[2] = {.text = (char *)"banana pie"};
    test_items[3] = {.text = (char *)"apple crumble"};
    test_items[4] = {.text = (char *)"cherry"};
    test_items[5] = {.text = NULL};
    items = test_items;

    // Create a list of popular items
    popular_items = (struct item *)ecalloc(3, sizeof(struct item));
    popular_items[0] = {.text = (char *)"apple crumble", .out = 10};
    popular_items[1] = {.text = (char *)"apple pie", .out = 5};
    popular_items[2] = {.text = NULL};
    popitems = popular_items;

    // Mock the underlying functions called by calcoffsets
    EXPECT_CALL(xlib_mock, XftTextExtentsUtf8)
        .WillRepeatedly(testing::Return());
  }

  void TearDown() override {
    free(test_items);
    free(popular_items);
    DmenuTest::TearDown();
  }
};

TEST_F(MatchTest, FindsExactMatches) {
  strcpy(text, "apple");

  match();

  // TODO: This is a bug: Crumble should be first
  ASSERT_NE(matches, (struct item *)NULL);
  EXPECT_STREQ(matches->text, "apple pie");
  EXPECT_STREQ(matches->right->text, "apple crumble");
  EXPECT_STREQ(matches->right->right->text, "apple");
}

TEST_F(MatchTest, FindsTokenMatches) {
  strcpy(text, "pie apple"); // Tokens are "pie" and "apple"

  match();

  ASSERT_NE(matches, (struct item *)NULL);
  EXPECT_STREQ(matches->text, "apple pie");
  EXPECT_EQ(matches->right, (struct item *)NULL);
}

TEST_F(MatchTest, HandlesNoMatches) {
  strcpy(text, "zucchini");

  match();

  EXPECT_EQ(matches, (struct item *)NULL);
}

TEST_F(MatchTest, SortsOtherMatchesByPopularity) {
  strcpy(text, "a");

  match();

  ASSERT_NE(matches, (struct item *)NULL);
  EXPECT_STREQ(matches->text, "apple crumble");
  EXPECT_STREQ(matches->right->text, "apple pie");
  EXPECT_STREQ(matches->right->right->text, "banana pie");
  EXPECT_STREQ(matches->right->right->right->text, "apple");
}

class InsertTest : public DmenuTest {
protected:
  void SetUp() override {
    DmenuTest::SetUp();

    // Initialize the global text buffer and cursor for each test
    strcpy(text, "hello world");
    cursor = 6; // Position cursor after "hello "

    // Mock the functions called by match() to avoid side effects
    EXPECT_CALL(xlib_mock, XftTextExtentsUtf8)
        .WillRepeatedly(testing::Return());
  }
};

TEST_F(InsertTest, InsertsTextAtCursor) {
  const char *new_str = "new ";

  insert(new_str, strlen(new_str));

  // Check the final state of the text and cursor
  EXPECT_STREQ(text, "hello new world");
  EXPECT_EQ(cursor, 10); // 6 (original) + 4 (new)
}

TEST_F(InsertTest, DeletesTextAtCursor) {
  cursor = 5; // Position cursor at the space

  insert(NULL, -6);
  EXPECT_STREQ(text, "world");
  EXPECT_EQ(cursor, static_cast<std::size_t>(0) - 1);
}

TEST_F(InsertTest, DoesNotInsertIfTooLong) {
  strcpy(text, "some text");
  cursor = strlen(text);

  char long_str[sizeof(text)];
  memset(long_str, 'a', sizeof(long_str));
  long_str[sizeof(long_str) - 1] = '\0';

  insert(long_str, strlen(long_str));

  EXPECT_STREQ(text, "some text");
  EXPECT_EQ(cursor, strlen(text));
}

TEST_F(InsertTest, HandlesZeroLengthInsert) {
  insert("nothing", 0);

  EXPECT_STREQ(text, "hello world");
  EXPECT_EQ(cursor, 6);
}

TEST_F(InsertTest, CallsMatchAfterOperation) {
  EXPECT_CALL(xlib_mock, XftTextExtentsUtf8).Times(testing::AtLeast(1));

  insert("test", 4);
}

class NextRuneTest : public DmenuTest {
protected:
  void SetUp() override {
    DmenuTest::SetUp();
    // Clear the text buffer before each test
    memset(text, 0, sizeof(text));
  }
};

TEST_F(NextRuneTest, MovesForwardOverAscii) {
  strcpy(text, "test");
  cursor = 1; // Cursor is at 'e'

  size_t next_pos = nextrune(+1);
  EXPECT_EQ(next_pos, 2); // Should move to 's'
}

TEST_F(NextRuneTest, MovesBackwardOverAscii) {
  strcpy(text, "test");
  cursor = 2; // Cursor is at 's'

  size_t next_pos = nextrune(-1);
  EXPECT_EQ(next_pos, 1); // Should move back to 'e'
}

TEST_F(NextRuneTest, MovesForwardOverMultiByte) {
  strcpy(text, "aŻb"); // Total length is 1 (a) + 2 (Ż) + 1 (b) = 4 bytes
  cursor = 0;          // Cursor is at 'a'

  size_t next_pos = nextrune(+1);
  EXPECT_EQ(next_pos, 1);
}

TEST_F(NextRuneTest, MovesBackwardOverMultiByte) {
  strcpy(text, "aŻb");
  cursor = 3; // Cursor is at 'b'

  size_t next_pos = nextrune(-1);
  EXPECT_EQ(next_pos, 1); // Position of the start of "Ż"
}

TEST_F(NextRuneTest, StaysAtBeginning) {
  strcpy(text, "test");
  cursor = 0;

  size_t next_pos = nextrune(-1);
  EXPECT_EQ(next_pos, static_cast<std::size_t>(0) - 1);
}

TEST_F(NextRuneTest, MovesFromInsideMultiByteChar) {
  strcpy(text, "aŻb");
  cursor = 2; // Cursor is on the *second* byte of "Ż"

  size_t next_pos_fwd = nextrune(+1);
  EXPECT_EQ(next_pos_fwd, 3);

  size_t next_pos_back = nextrune(-1);
  EXPECT_EQ(next_pos_back, 1);
}

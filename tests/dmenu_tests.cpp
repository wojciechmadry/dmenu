#include "gtest/gtest.h"
#include "mocks/all_mock_class.hpp"
#include "mocks/X11/X.h"
#include "mocks/X11/Xlib.h"
#include "mocks/X11/extensions/Xinerama.h"

#include "dmenu_lib.h"

// STD
#include <iostream>

TEST(dmenuTest, macros) {
        XineramaScreenInfo xsi{};
        xsi.x_xorg = 5;
        xsi.y_xorg = 4;
        xsi.width = 3;
        xsi.height = 6;
        std::cout << INTERSECT(-1, -1, 1, 1, r)
	EXPECT_TRUE(false);
}

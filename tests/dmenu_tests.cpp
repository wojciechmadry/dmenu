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
        xsi.x_org = 5;
        xsi.y_org = 4;
        xsi.width = 3;
        xsi.height = 6;
        EXPECT_EQ(INTERSECT(-1, -1, 1, 1, xsi), 0);
}

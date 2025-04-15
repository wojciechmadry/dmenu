#ifndef MOCK_XFTFONT_HPP_
#define MOCK_XFTFONT_HPP_

#include "fccharset.hpp"
#include "fcpattern.hpp"

class XftFont{
public:
    int		ascent;
    int		descent;
    int		height;
    int		max_advance_width;
    FcCharSet	*charset;
    FcPattern	*pattern;
};

#endif

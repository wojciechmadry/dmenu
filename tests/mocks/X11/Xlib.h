#ifndef MOCK_XLIB_HPP_
#define MOCK_XLIB_HPP_

#include <gmock/gmock.h>
#include "all_mock_class.hpp"
#include "X.h"
#include <cstdint>

typedef std::uint64_t Pixmap;
#define False 0

#define ScreenOfDisplay(dpy, scr)(&dpy->screens[scr])
#define DefaultVisual(dpy, scr) (ScreenOfDisplay(dpy,scr)->root_visual)
#define DefaultColormap(dpy, scr)(ScreenOfDisplay(dpy,scr)->cmap)
#define DefaultDepth(dpy, scr) 	(ScreenOfDisplay(dpy,scr)->root_depth)

namespace impl{
class XlibResults_ {
    public:
    virtual int XCreatePixmap() = 0;
    virtual GC XCreateGC() = 0;
    virtual int XSetLineAttributes() = 0;
    virtual int XFreePixmap() = 0;
};

struct XlibMockResults_ : public XlibResults_ {
    MOCK_METHOD(int, XCreatePixmap, (), (override));
    MOCK_METHOD(GC, XCreateGC, (), (override));
    MOCK_METHOD(int, XSetLineAttributes, (), (override));
    MOCK_METHOD(int, XFreePixmap, (), (override));
};
}

impl::XlibMockResults_& getXlibMockResults();

typedef struct _XGlyphInfo {
    /** Glyph width. */
    unsigned short width;
    /** Glyph height. */
    unsigned short height;

    /** Horizontal Glyph center offset relative to the upper-left corner. */
    short x;
    /** Vertical Glyph center offset relative to the upper-left corner. */
    short y;

    /** Horizontal margin to the next Glyph. */
    short xOff;
    /** Vertical margin to the next Glyph. */
    short yOff;
} XGlyphInfo;

inline int XSetForeground(
    Display*		/* display */,
    GC			/* gc */,
    unsigned long	/* foreground */
) {
  return 0;
}

inline int XFillRectangle(
    Display*		/* display */,
    Drawable		/* d */,
    GC			/* gc */,
    int			/* x */,
    int			/* y */,
    unsigned int	/* width */,
    unsigned int	/* height */
){
  return 0;
}

inline int XDrawRectangle(
    Display*		/* display */,
    Drawable		/* d */,
    GC			/* gc */,
    int			/* x */,
    int			/* y */,
    unsigned int	/* width */,
    unsigned int	/* height */
) {
  return 0;
}

inline int XCopyArea(
    Display*		/* display */,
    Drawable		/* src */,
    Drawable		/* dest */,
    GC			/* gc */,
    int			/* src_x */,
    int			/* src_y */,
    unsigned int	/* width */,
    unsigned int	/* height */,
    int			/* dest_x */,
    int			/* dest_y */
) {
  return 0;
}

inline int XFreeGC(
    Display*		/* display */,
    GC			/* gc */
) {
  return 0;
}

inline int XFreePixmap(
    Display*		/* display */,
    Pixmap		/* pixmap */
) {
  return getXlibMockResults().XFreePixmap();
}

inline int XSync(
    Display*		/* display */,
    bool		/* discard */
) {return 0;}

inline int XFreeCursor(
    Display*		/* display */,
    Cursor		/* cursor */
) {return 0;}

inline Cursor XCreateFontCursor(
    Display*		/* display */,
    unsigned int	/* shape */
) {return {};}

inline Pixmap XCreatePixmap(
    Display*		/* display */,
    Drawable		/* d */,
    unsigned int	/* width */,
    unsigned int	/* height */,
    unsigned int	/* depth */
) {
    return getXlibMockResults().XCreatePixmap();
}


struct XGCValues{};

inline GC XCreateGC(
    Display*		/* display */,
    Drawable		/* d */,
    unsigned long	/* valuemask */,
    XGCValues*		/* values */
) {return getXlibMockResults().XCreateGC();}

inline int XSetLineAttributes(
    Display*		/* display */,
    GC			/* gc */,
    unsigned int	/* line_width */,
    int			/* line_style */,
    int			/* cap_style */,
    int			/* join_style */
) {return getXlibMockResults().XSetLineAttributes();}

#endif

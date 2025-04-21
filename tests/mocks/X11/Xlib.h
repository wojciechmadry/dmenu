#ifndef MOCK_XLIB_HPP_
#define MOCK_XLIB_HPP_

#include <gmock/gmock.h>
#include "all_mock_class.hpp"
#include "X.h"
#include <cstdint>

typedef std::uint64_t Pixmap;

typedef std::int8_t	    XftChar8;
typedef std::uint64_t    XftResult;
typedef XftResult     FcResult;

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
    virtual int XFreeGC() = 0;
    virtual XftFont* XftFontOpenPattern() = 0;
    virtual XftFont* XftFontOpenName() = 0;
    virtual FcPattern* FcNameParse() = 0;
    virtual void XftTextExtentsUtf8() = 0;
    virtual bool XftColorAllocName() = 0;
    virtual Cursor XCreateFontCursor() = 0;
    virtual int XFreeCursor() = 0;
    virtual int XCopyArea() = 0;
    virtual int XSync() = 0;
    virtual int XSetForeground() = 0;
    virtual int XFillRectangle() = 0;
    virtual int XDrawRectangle() = 0;
    virtual FcPattern* XftFontMatch() = 0;
};

struct XlibMockResults_ : public XlibResults_ {
    MOCK_METHOD(int, XCreatePixmap, (), (override));
    MOCK_METHOD(GC, XCreateGC, (), (override));
    MOCK_METHOD(int, XSetLineAttributes, (), (override));
    MOCK_METHOD(int, XFreePixmap, (), (override));
    MOCK_METHOD(int, XFreeGC, (), (override));
    MOCK_METHOD(XftFont*, XftFontOpenPattern, (), (override));
    MOCK_METHOD(XftFont*, XftFontOpenName, (), (override));
    MOCK_METHOD(FcPattern*, FcNameParse, (), (override));
    MOCK_METHOD(void, XftTextExtentsUtf8, (), (override));
    MOCK_METHOD(bool, XftColorAllocName, (), (override));
    MOCK_METHOD(Cursor, XCreateFontCursor, (), (override));
    MOCK_METHOD(int, XFreeCursor, (), (override));
    MOCK_METHOD(int, XCopyArea, (), (override));
    MOCK_METHOD(int, XSync, (), (override));
    MOCK_METHOD(int, XSetForeground, (), (override));
    MOCK_METHOD(int, XFillRectangle, (), (override));
    MOCK_METHOD(int, XDrawRectangle, (), (override));
    MOCK_METHOD(FcPattern*, XftFontMatch, (), (override));
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

inline XftFont *
XftFontOpenPattern (Display *dpy, FcPattern *pattern) {
  return getXlibMockResults().XftFontOpenPattern();
}

inline int XSetForeground(
    Display*		/* display */,
    GC			/* gc */,
    unsigned long	/* foreground */
) {
  return getXlibMockResults().XSetForeground();
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
  return getXlibMockResults().XFillRectangle();
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
  return getXlibMockResults().XDrawRectangle();
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
  return getXlibMockResults().XCopyArea();
}

inline int XFreeGC(
    Display*		/* display */,
    GC			/* gc */
) {
  return getXlibMockResults().XFreeGC();
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
) {return getXlibMockResults().XSync();}

inline int XFreeCursor(
    Display*		/* display */,
    Cursor		/* cursor */
) {return getXlibMockResults().XFreeCursor();}

inline Cursor XCreateFontCursor(
    Display*		/* display */,
    unsigned int	/* shape */
) {return getXlibMockResults().XCreateFontCursor();}

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

#include <cstdint>

inline void
XftTextExtentsUtf8 (Display	    *dpy,
		    XftFont	    *pub,
		    const FcChar8 *string,
		    int		    len,
		    XGlyphInfo	    *extents){
    if(extents) {
	*extents = XGlyphInfo{};
    }
    getXlibMockResults().XftTextExtentsUtf8();
}

inline void
XftFontClose (Display *dpy, XftFont *pub) {
  if(pub){
    free(pub);
  }
}

inline XftDraw *
XftDrawCreate (Display   *dpy,
	       Drawable  drawable,
	       Visual    *visual,
	       Colormap  colormap) {
  return nullptr;
}

inline bool
XftCharExists (Display	    *dpy,
	       XftFont	    *pub,
	       FcChar32    ucs4) {
  return true;
}

inline bool
XftColorAllocName (Display  *dpy,
		   const Visual   *visual,
		   Colormap cmap,
		   const char	    *name,
		   XftColor *result) {
  return getXlibMockResults().XftColorAllocName();
}

inline void
XftDrawDestroy (XftDraw	*draw) {}

inline void
XftDrawStringUtf8 (XftDraw	    *draw,
		   const XftColor *color,
		   XftFont	    *pub,
		   int		    x,
		   int		    y,
		   const FcChar8  *string,
		   int		    len) {}

inline XftFont *
XftFontOpenName (Display *dpy, int screen, const char *name) {
  return getXlibMockResults().XftFontOpenName();
}

inline FcPattern *
FcNameParse (const FcChar8 *name) {
  return getXlibMockResults().FcNameParse();
}

inline FcPattern *
XftFontMatch (Display		*dpy,
	      int		screen,
	      const FcPattern *pattern,
	      FcResult		*result) {return getXlibMockResults().XftFontMatch();}


#endif

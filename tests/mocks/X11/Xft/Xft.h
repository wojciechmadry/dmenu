#ifndef MOCK_XFT_HPP_
#define MOCK_XFT_HPP_

#include "all_mock_class.hpp"
#include <cstdint>

typedef std::int8_t	    XftChar8;
typedef std::uint64_t    XftResult;
typedef XftResult     FcResult;

inline void
XftTextExtentsUtf8 (Display	    *dpy,
		    XftFont	    *pub,
		    const FcChar8 *string,
		    int		    len,
		    XGlyphInfo	    *extents){}

inline void
XftFontClose (Display *dpy, XftFont *pub) {}

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
  return true;
}

inline XftFont *
XftFontOpenPattern (Display *dpy, FcPattern *pattern) {
  return nullptr;
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
  return nullptr;
}

inline FcPattern *
XftFontMatch (Display		*dpy,
	      int		screen,
	      const FcPattern *pattern,
	      FcResult		*result) {return nullptr;}
#endif

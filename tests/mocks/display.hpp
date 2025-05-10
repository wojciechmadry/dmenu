#ifndef MOCK_DISPLAY_HPP_
#define MOCK_DISPLAY_HPP_

#include <cstdint>
using Colormap = std::uint64_t;

using Visual = int;

class Screen{
public:
  Colormap cmap;
  Visual *root_visual;	/* root visual */
  int root_depth;		/* bits per pixel */
};

class Display{
public:
  Screen* screens;
};

#endif

#ifndef MOCK_XFTCOLOR_HPP_
#define MOCK_XFTCOLOR_HPP_

#include <cstdint>

typedef struct {
    /** Red color channel. */
    std::uint16_t red;
    /** Green color channel. */
    std::uint16_t  green;
    /** Blue color channel. */
    std::uint16_t  blue;
    /** Alpha color channel. */
    std::uint16_t  alpha;
} XRenderColor;

class XftColor{
public:
  std::uint64_t pixel;
  XRenderColor color;
};

#endif

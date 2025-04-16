#include "Xlib.h"

impl::XlibMockResults_& getXlibMockResults() {
  static impl::XlibMockResults_ XlibMockResults{};
  return XlibMockResults;
}


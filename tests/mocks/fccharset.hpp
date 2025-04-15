#ifndef MOCK_FCCHARSET_HPP_
#define MOCK_FCCHARSET_HPP_

#include <cstdint>

typedef std::int8_t	    FcChar8;
typedef std::int32_t	    FcChar32;

typedef enum _FcMatchKind {
    FcMatchPattern,
    FcMatchFont,
    FcMatchScan,
    FcMatchKindEnd,
    FcMatchKindBegin = FcMatchPattern
} FcMatchKind;

class FcCharSet{

};

inline FcCharSet *
FcCharSetCreate (void) {
  return nullptr;
}

inline bool
FcCharSetAddChar (FcCharSet *fcs, FcChar32 ucs4) {
  return true;
}
#endif

#ifndef MOCK_FCPATTERN_HPP_
#define MOCK_FCPATTERN_HPP_

#include "fccharset.hpp"

#define FC_SCALABLE             "scalable"       /* Bool */
#define FC_CHARSET              "charset"        /* CharSet */
#define FcTrue                  1

class FcPattern{

};

class FcConfig{

};

inline void
FcPatternDestroy (FcPattern *p){
  if(p) {
    free(p);
  }
}

inline FcPattern *
FcPatternDuplicate (const FcPattern *p) {return nullptr;}

inline bool
FcPatternAddBool (FcPattern *p, const char *object, bool b) {
  return true;
}

inline bool
FcPatternAddCharSet (FcPattern *p, const char *object, const FcCharSet *c) {return true;}

inline void
FcDefaultSubstitute (FcPattern *pattern) {}

inline void
FcCharSetDestroy (FcCharSet *fcs) {}

inline FcPattern *
FcNameParse (const FcChar8 *name) {
  return nullptr;
}

inline bool FcConfigSubstitute (FcConfig   *config,
                    FcPattern  *p,
                    FcMatchKind kind) {
  return true;
}

#endif

#ifndef MOCK_X_HPP_
#define MOCK_X_HPP_

#include "all_mock_class.hpp"

#include <cstdint>
#define LineSolid		0
#define CapButt			1
#define JoinMiter		0
#define CurrentTime 123
#define RevertToParent 1
#define CopyFromParent NULL
#define ControlMask 1
#define ShiftMask 2
#define Mod1Mask 4
#define XK_Home 100
#define XK_Left 101
#define XK_Escape 102
#define XK_End 103
#define XK_e 104
#define XK_Delete 105
#define XK_d 106
#define XK_a 107
#define XK_BackSpace 108
#define XK_h 109
#define XK_g 110
#define XK_f 111
#define XK_c 112
#define XK_b 113
#define XK_m 114
#define XK_M 115
#define XK_j 116
#define XK_p 118
#define XK_k 119
#define XK_Down 120
#define XK_Tab 121
#define XK_Right 122
#define XK_i 123
#define XK_w 124
#define XK_u 125
#define XK_KP_Left 126
#define XK_y 127
#define XK_n 128
#define XK_J 129
#define XK_XK_Return 130
#define XK_KP_Enter 131
#define XK_Return 132
#define XK_Up 133
#define XK_Y 134
#define XK_Prior 135
#define XK_Next 136
#define XK_G 137
#define XK_KP_End 138
#define XK_KP_Delete 139
#define XK_l 140
#define XK_bracketleft 141
#define XK_KP_Prior 142
#define XK_KP_Next 143
#define XK_KP_Up 144
#define XK_KP_Home 145
#define XK_KP_Down 146
#define XK_KP_Right 147
#define XLookupKeySym 148
#define XLookupBoth 149
#define XLookupChars 150
#define NoSymbol 0
#define DestroyNotify 1
#define Expose 2
#define KeyPress 3
#define FocusIn 4
#define VisibilityNotify 5
#define SelectionNotify 6
#define VisibilityUnobscured 7
#define Success 0
#define GrabSuccess 0
#define GrabModeAsync 1
#define True 1
#define None 0
#define PointerRoot 10
#define ExposureMask 8
#define KeyPressMask 16
#define VisibilityChangeMask 32
#define CWOverrideRedirect 64
#define CWBackPixel 128
#define CWEventMask 256
#define SubstructureNotifyMask 512
#define FocusChangeMask 1024
#define XNInputStyle 1
#define XIMPreeditNothing 2
#define XIMStatusNothing 4
#define XNFocusWindow 5
#define XNClientWindow 5

typedef unsigned long Atom;
typedef unsigned long XIC;
typedef unsigned long XIM;
typedef int KeySym;

#endif

//
// Created by shayne on 11/9/16.
//

#ifndef SHAKE_COLORS_H
#define SHAKE_COLORS_H

#define TCOLOR(C, S) C S TC_CLEAR
#define TC_CLEAR "\x1b[0m"
#define TC_RED(S) TCOLOR("\x1b[31m", S)
#define TC_GREEN(S) TCOLOR("\x1b[32m", S)
#define TC_YELLOW(S) TCOLOR("\x1b[33m", S)
#define TC_BLUE(S) TCOLOR("\x1b[34m", S)
#define TC_MAGENTA(S) TCOLOR("\x1b[35m", S)
#define TC_CYAN(S) TCOLOR("\x1b[36m", S)

#endif // SHAKE_COLORS_H

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

#define ANSI_GREEN(S) TCOLOR("\033[38;5;10m", S)
#define ANSI_GRAY(S) TCOLOR("\033[38;5;246m", S)
#define ANSI_PINK(S) TCOLOR("\033[38;5;207m", S)
#define ANSI_BOLD(S) TCOLOR("\033[1m", S)
#define ANSI_END(S) TCOLOR("\033[0m", S)
#define COLOR_DEBUG(S) TCOLOR("\033[1;36m", S)
#define COLOR_INFO(S) TCOLOR("\033[0;32m", S)
#define COLOR_WARN(S) TCOLOR("\033[1;33m", S)
#define COLOR_ERROR(S) TCOLOR("\033[0;31m", S)
#define COLOR_INSTRUCTIONS(S) TCOLOR("\033[0;37m", S)

#endif // SHAKE_COLORS_H

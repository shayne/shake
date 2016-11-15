//
// Created by shayne on 11/9/16.
//

#ifndef SHAKE_COLORS_H
#define SHAKE_COLORS_H

#define ANSI_COLOR(C, S) C S ANSI_CLEAR
#define ANSI_CLEAR "\x1b[0m"
//#define ANSI_CLEAR "\033[0m"

#define ANSI_GREEN(S) ANSI_COLOR("\033[38;5;10m", S)
#define ANSI_GRAY(S) ANSI_COLOR("\033[38;5;246m", S)
#define ANSI_PINK(S) ANSI_COLOR("\033[38;5;207m", S)
#define ANSI_BOLD(S) ANSI_COLOR("\033[1m", S)
#define COLOR_DEBUG(S) ANSI_COLOR("\033[1;36m", S)
#define COLOR_INFO(S) ANSI_COLOR("\033[0;32m", S)
#define COLOR_WARN(S) ANSI_COLOR("\033[1;33m", S)
#define COLOR_ERROR(S) ANSI_COLOR("\033[0;31m", S)
#define COLOR_INSTRUCTIONS(S) ANSI_COLOR("\033[0;37m", S)

#endif // SHAKE_COLORS_H

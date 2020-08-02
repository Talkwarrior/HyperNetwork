#pragma once
// 다음의 소스를 수정하여 사용하였습니다.
// https://github.com/sol-prog/ansi-escape-codes-windows-posix-terminals-c-programming-examples

#define RESET_COLOR "\x1b[0m"
#define BLACK_TXT	"\x1b[30m"
#define RED_TXT		"\x1b[31m"
#define GREEN_TXT	"\x1b[32m"
#define YELLOW_TXT	"\x1b[33m"
#define BLUE_TXT	"\x1b[34m"
#define MAGENTA_TXT	"\x1b[35m"
#define CYAN_TXT	"\x1b[36m"
#define WHITE_TXT	"\x1b[37m"


#define BLACK_BKG	"\x1b[40m"
#define RED_BKG		"\x1b[41m"
#define GREEN_BKG	"\x1b[42m"
#define YELLOW_BKG	"\x1b[43m"
#define BLUE_BKG	"\x1b[44m"
#define MAGENTA_BKG	"\x1b[45m"
#define CYAN_BKG	"\x1b[46m"
#define WHITE_BKG	"\x1b[47m"

void setupConsole(void);
void restoreConsole(void);
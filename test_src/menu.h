/*
 * libslp-tapi
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Ja-young Gu <jygu@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MENU_H__
#define __MENU_H__

__BEGIN_DECLS

#define ANSI_COLOR_NORMAL		"\e[0m"

#define ANSI_COLOR_BLACK		"\e[0;30m"
#define ANSI_COLOR_RED			"\e[0;31m"
#define ANSI_COLOR_GREEN		"\e[0;32m"
#define ANSI_COLOR_BROWN		"\e[0;33m"
#define ANSI_COLOR_BLUE			"\e[0;34m"
#define ANSI_COLOR_MAGENTA		"\e[0;35m"
#define ANSI_COLOR_CYAN			"\e[0;36m"
#define ANSI_COLOR_LIGHTGRAY	"\e[0;37m"

#define ANSI_COLOR_DARKGRAY		"\e[1;30m"
#define ANSI_COLOR_LIGHTRED		"\e[1;31m"
#define ANSI_COLOR_LIGHTGREEN	"\e[1;32m"
#define ANSI_COLOR_YELLOW		"\e[1;33m"
#define ANSI_COLOR_LIGHTBLUE	"\e[1;34m"
#define ANSI_COLOR_LIGHTMAGENTA "\e[1;35m"
#define ANSI_COLOR_LIGHTCYAN	"\e[1;36m"
#define ANSI_COLOR_WHITE		"\e[1;37m"


#define msg(fmt,args...)	{ fprintf(stdout, fmt "\n", ##args); \
		fflush(stdout); }

#define msgn(fmt,args...)	{ fprintf(stdout, fmt, ##args); \
		fflush(stdout); }

/* Bold (green) */
#define msgb(fmt,args...)  { fprintf(stdout, ANSI_COLOR_LIGHTGREEN fmt \
		ANSI_COLOR_NORMAL "\n", ##args); fflush(stdout); }

/* Property message */
#define msgp(fmt,args...)  { fprintf(stdout, ANSI_COLOR_LIGHTMAGENTA fmt \
		ANSI_COLOR_NORMAL "\n", ##args); fflush(stdout); }

#define msgt(n,fmt,args...)	{ fprintf(stdout, "\e[%dC" fmt "\n", \
		3 + ((n) * 2), ##args); fflush(stdout); }

#define pmsg(fmt,args...)	{ \
	if (is_pid_show()) { fprintf(stdout, "(%5d) ", get_tid()); } \
	fprintf(stdout, fmt "\n", ##args); fflush(stdout); }

#define pmsgb(fmt,args...)	{ \
	if (is_pid_show()) { fprintf(stdout, "(%5d) ", get_tid()); } \
	fprintf(stdout, ANSI_COLOR_LIGHTGREEN fmt \
			ANSI_COLOR_NORMAL "\n", ##args); fflush(stdout); }

#define pmsgt(n,fmt,args...) { \
	if (is_pid_show()) { fprintf(stdout, "(%5d) ", get_tid()); } \
	fprintf(stdout, "\e[%dC" fmt "\n", \
			3 + ((n) * 2), ##args); fflush(stdout); }

#define MENU_DATA_SIZE 255



/*
 * Horizontal Line - width: 65
 *					.12345678901234567890123456789012345678901234567890.
 */
#define HR_SINGLE	"----------------------------------------" \
					"-------------------------"
#define HR_DOUBLE	"========================================" \
					"========================="
#define HR_SINGLE2	" ---------------------------------------" \
					"------------------------ "

#define MAX_WIDTH	strlen (HR_SINGLE)
#define MAX_TITLE	((MAX_WIDTH) - 10)
#define POS_MORE	((MAX_WIDTH) - 3)
#define RET_SUCCESS 0
#define RET_FAILURE -1

typedef struct menu_manager MManager;

struct menu_data {
	const char *key;
	const char *title;
	struct menu_data *sub_menu;
	int (*callback)(MManager *mm, struct menu_data *menu);
	char *data;
};

MManager* menu_manager_new(struct menu_data items[], GMainLoop *mainloop);
int       menu_manager_run(MManager *mm);
int       menu_manager_set_user_data(MManager *mm, void *user_data);
void*     menu_manager_ref_user_data(MManager *mm);

gboolean  on_menu_manager_keyboard(GIOChannel *src, GIOCondition con,
		gpointer data);

pid_t     get_tid ();
void      hide_pid ();
void      show_pid ();
int       is_pid_show ();
void      menu_print_dump(int data_len, void *data);

__END_DECLS

#endif

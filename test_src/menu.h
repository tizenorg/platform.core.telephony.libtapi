/*
 * libtapi
 *
 * Copyright (c) 2013 Samsung Electronics Co. Ltd. All rights reserved.
 * Copyright (c) 2013 Intel Corporation. All rights reserved.
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

#define ANSI_COLOR_NORMAL "\e[0m"

#define ANSI_COLOR_BLACK "\e[0;30m"
#define ANSI_COLOR_RED "\e[0;31m"
#define ANSI_COLOR_GREEN "\e[0;32m"
#define ANSI_COLOR_BROWN "\e[0;33m"
#define ANSI_COLOR_BLUE "\e[0;34m"
#define ANSI_COLOR_MAGENTA "\e[0;35m"
#define ANSI_COLOR_CYAN "\e[0;36m"
#define ANSI_COLOR_LIGHTGRAY "\e[0;37m"

#define ANSI_COLOR_DARKGRAY "\e[1;30m"
#define ANSI_COLOR_LIGHTRED "\e[1;31m"
#define ANSI_COLOR_LIGHTGREEN "\e[1;32m"
#define ANSI_COLOR_YELLOW "\e[1;33m"
#define ANSI_COLOR_LIGHTBLUE "\e[1;34m"
#define ANSI_COLOR_LIGHTMAGENTA "\e[1;35m"
#define ANSI_COLOR_LIGHTCYAN "\e[1;36m"
#define ANSI_COLOR_WHITE "\e[1;37m"

#define msg(fmt,args...) do { fprintf(stdout, fmt "\n", ##args); fflush(stdout); } while (0)
#define msgn(fmt,args...) do { fprintf(stdout, fmt, ##args); fflush(stdout); } while (0)
#define msgb(fmt,args...) do { fprintf(stdout, ANSI_COLOR_LIGHTGREEN fmt ANSI_COLOR_NORMAL "\n", ##args); fflush(stdout); } while (0)

#define MENU_DATA_SIZE 	255

#define CHECK_RT(x) \
{ \
	if (x != TEL_RETURN_SUCCESS) { \
		msg("api call failed with [%d]", x); \
		return 0; \
	} \
}

#define RET_SUCCESS 0
#define RET_FAILURE -1

typedef struct menu_manager MManager;

struct menu_data {
	char *key;
	char *title;
	struct menu_data *sub_menu;
	int (*callback)(MManager *mm, struct menu_data *menu);
	char *data;
};

MManager* menu_manager_new(struct menu_data items[], GMainLoop *mainloop);
int       menu_manager_run(MManager *mm);
int       menu_manager_set_user_data(MManager *mm, void *user_data);
void*     menu_manager_ref_user_data(MManager *mm);

gboolean  on_menu_manager_keyboard(GIOChannel *src, GIOCondition con, gpointer data);

__END_DECLS

#endif

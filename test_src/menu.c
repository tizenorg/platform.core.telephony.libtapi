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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <glib.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "menu.h"

#define DEFAULT_MENU_MENU	"m"
#define DEFAULT_MENU_PREV	"p"
#define DEFAULT_MENU_QUIT	"q"
#define DEFAULT_MENU_NONE	"-"

struct menu_manager {
	GQueue *stack;
	GQueue *title_stack;

	struct menu_data *menu;

	char *buf;

	void *user_data;
	GMainLoop *mainloop;
};


char key_buffer[MENU_DATA_SIZE];
int flag_pid_display = 1;


static void _show_prompt (void)
{
	msgn ("(%5d) >> ", get_tid ());
}

static void _show_reserved_menu (void)
{
	msg (ANSI_COLOR_DARKGRAY HR_SINGLE2 ANSI_COLOR_NORMAL);
	msg (ANSI_COLOR_DARKGRAY " [ " ANSI_COLOR_NORMAL "%s" ANSI_COLOR_DARKGRAY
			" ] " ANSI_COLOR_NORMAL "Previous menu " , DEFAULT_MENU_PREV);
	msg (ANSI_COLOR_DARKGRAY " [ " ANSI_COLOR_NORMAL "%s" ANSI_COLOR_DARKGRAY
			" ] " ANSI_COLOR_NORMAL "Show Menu " , DEFAULT_MENU_MENU);
	msg (ANSI_COLOR_DARKGRAY " [ " ANSI_COLOR_NORMAL "%s" ANSI_COLOR_DARKGRAY
			" ] " ANSI_COLOR_NORMAL "Quit " , DEFAULT_MENU_QUIT);
}

static void _show_input_ok (void)
{
	msg ("OK.");
}

static void _show_menu (MManager *m, struct menu_data menu[])
{
	int i = 0;
	int len = 0;
	struct menu_data *item;
	char title_buf[256] = { 0, };

	if (!menu)
		return;

	msg ("");
	msg (HR_DOUBLE);

	len = g_queue_get_length (m->title_stack);
	msgn (ANSI_COLOR_YELLOW " Main");
	if (len > 0) {
		for (i = 0; i < len; i++) {
			msgn (ANSI_COLOR_NORMAL " >> " ANSI_COLOR_YELLOW "%s",
					(char *) g_queue_peek_nth (m->title_stack, i));
		}
	}
	msg (ANSI_COLOR_NORMAL);
	msg (HR_SINGLE);

	hide_pid();
	i = 0;

	while (1) {
		item = menu + i;
		if (item->key == NULL)
			break;

		if (!g_strcmp0 (item->key, "-")) {
			msgn ("       ");
		}
		else if (!g_strcmp0 (item->key, "_")) {
			msg (ANSI_COLOR_DARKGRAY HR_SINGLE2 ANSI_COLOR_NORMAL);

			if (item->callback)
				item->callback (m, item);

			i++;

			continue;
		}
		else if (!g_strcmp0 (item->key, "*")) {
			msg (" %s", item->title);
			if (item->callback)
				item->callback (m, item);
		}
		else {
			msgn (ANSI_COLOR_DARKGRAY " [" ANSI_COLOR_NORMAL "%3s"
					ANSI_COLOR_DARKGRAY "] " ANSI_COLOR_NORMAL,	item->key);
		}

		memset (title_buf, 0, 256);
		if (item->title) {
			snprintf (title_buf, MAX_TITLE, "%s", item->title);

			if (strlen (item->title) >= MAX_TITLE) {
				title_buf[MAX_TITLE - 2] = '.';
				title_buf[MAX_TITLE - 1] = '.';
			}
		}

		if (item->data) {
			msg ("%s " ANSI_COLOR_LIGHTBLUE "(%s)" ANSI_COLOR_NORMAL,
					title_buf, item->data);
		}
		else if (!g_strcmp0 (item->key, "*")) {
			/* none */
		}
		else {
			msg ("%s", title_buf);
		}

		if (item->sub_menu) {
			msg ("\e[1A\e[%dC >", (int)POS_MORE);
		}

		i++;
	}

	show_pid();

	_show_reserved_menu();

	msg (HR_DOUBLE);

	_show_prompt();
}

static void _show_item_data_input_msg (struct menu_data *item)
{
	msg ("");
	msg (HR_DOUBLE);
	msg (" Input [%s] data ", item->title);
	msg (HR_SINGLE);
	msg (" current = [%s]", item->data);
	msgn (" new >> ");
}

static void _move_menu (MManager *mm, struct menu_data menu[], char *key)
{
	struct menu_data *item;
	int i = 0;

	if (!mm->menu)
		return;

	if (!g_strcmp0 (DEFAULT_MENU_PREV, key)) {
		if (g_queue_get_length (mm->stack) > 0) {
			mm->menu = g_queue_pop_tail (mm->stack);
			g_queue_pop_tail (mm->title_stack);
		}

		_show_menu (mm, mm->menu);
		mm->buf = key_buffer;

		return;
	}
	else if (!g_strcmp0 (DEFAULT_MENU_MENU, key)) {
		_show_menu (mm, mm->menu);
		return;
	}
	else if (!g_strcmp0 (DEFAULT_MENU_QUIT, key)) {
		g_main_loop_quit (mm->mainloop);
		return;
	}
	else if (!g_strcmp0 (DEFAULT_MENU_NONE, key)) {
		_show_prompt();
		return;
	}

	while (1) {
		int ret = RET_SUCCESS;
		item = menu + i;
		if (item->key == NULL)
			break;

		if (!g_strcmp0 (item->key, key)) {
			if (item->callback) {
				ret = item->callback (mm, item);
				_show_prompt();
			}

			if(RET_SUCCESS == ret) {
				if (item->sub_menu) {
					g_queue_push_tail (mm->stack, mm->menu);
					g_queue_push_tail (mm->title_stack, (gpointer *)item->title);

					mm->menu = item->sub_menu;
					_show_menu (mm, mm->menu);
					mm->buf = key_buffer;
				}

				if (item->data) {
					_show_item_data_input_msg (item);
					mm->buf = item->data;
				}
			}

			return;
		}

		i++;
	}

	_show_prompt();
}

MManager *menu_manager_new (struct menu_data items[], GMainLoop *mainloop)
{
	MManager *mm;

	mm = calloc (sizeof (struct menu_manager), 1);
	if (!mm)
		return NULL;

	mm->stack = g_queue_new();
	mm->title_stack = g_queue_new();
	mm->menu = items;
	mm->mainloop = mainloop;

	return mm;
}

int menu_manager_run (MManager *mm)
{
	_show_menu (mm, mm->menu);

	mm->buf = key_buffer;

	return 0;
}

int menu_manager_set_user_data (MManager *mm, void *user_data)
{
	if (!mm)
		return -1;

	mm->user_data = user_data;

	return 0;
}

void *menu_manager_ref_user_data (MManager *mm)
{
	if (!mm)
		return NULL;

	return mm->user_data;
}

gboolean on_menu_manager_keyboard (GIOChannel *src, GIOCondition con,
		gpointer data)
{
	MManager *mm = data;
	char local_buf[MENU_DATA_SIZE + 1] = { 0, };

	if (fgets (local_buf, MENU_DATA_SIZE, stdin) == NULL)
		return TRUE;

	if (strlen (local_buf) > 0) {
		if (local_buf[strlen (local_buf) - 1] == '\n')
			local_buf[strlen (local_buf) - 1] = '\0';
	}

	if (mm->buf == key_buffer) {
		if (strlen (local_buf) < 1) {
			_show_prompt();
			return TRUE;
		}

		_move_menu (mm, mm->menu, local_buf);
	}
	else {
		if (mm->buf) {
			memset (mm->buf, 0, MENU_DATA_SIZE);
			memcpy (mm->buf, local_buf, MENU_DATA_SIZE);
			_show_input_ok();
		}
		mm->buf = key_buffer;
		_move_menu (mm, mm->menu, (char *)DEFAULT_MENU_MENU);
	}

	return TRUE;
}

pid_t get_tid ()
{
	return syscall (__NR_gettid);
}

void hide_pid ()
{
	flag_pid_display = 0;
}

void show_pid ()
{
	flag_pid_display = 1;
}

int is_pid_show ()
{
	return flag_pid_display;
}

static void _hex_dump(const char *pad, int size, const void *data)
{
	char buf[255] = {0, };
	char hex[4] = {0, };
	int i;
	unsigned char *p;

	if (size <= 0) {
		msg("%sno data", pad);
		return;
	}
	p = (unsigned char *)data;

	snprintf(buf, 255, "%s%04X: ", pad, 0);
	for (i = 0; i<size; i++) {
		snprintf(hex, 4, "%02X ", p[i]);
		strcat(buf, hex);

		if ((i + 1) % 8 == 0) {
			if ((i + 1) % 16 == 0) {
				msg("%s", buf);
				memset(buf, 0, 255);
				snprintf(buf, 255, "%s%04X: ", pad, i + 1);
			}
			else {
				strcat(buf, "  ");
			}
		}
	}

	msg("%s", buf);
}

void menu_print_dump(int data_len, void *data)
{
	if(!data)
		return;

	msg("");
	msg("  \tlen=%d", data_len);
	_hex_dump("        ", data_len, data);

	msg("");
}


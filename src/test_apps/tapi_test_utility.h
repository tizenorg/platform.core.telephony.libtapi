/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyeongchul Kim <kyeongchul.kim@samsung.com>
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

#ifndef _TAPI_TEST_UTILITY_H_
#define _TAPI_TEST_UTILITY_H_


#define AUTO_LOG(frmt, args...) { \
	{do { fprintf(log_fd, "\n%s: " frmt "\n", __TIME__, ##args); } while (FALSE) ;}	\
	fflush(log_fd); }

#define TAPI_TEST_DEBUG(frmt, args...)	\
	{do { fprintf(stderr, "[TAPI Test][%s:%04d] "frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}

#endif

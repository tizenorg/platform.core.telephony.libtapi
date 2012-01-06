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

#ifndef TET_MACRO_H
#define TET_MACRO_H

#define TET_CHECK(x) if(x) tet_result(TET_PASS); else tet_result(TET_FAIL)
#define TET_CHECK_EQUAL(x,y) if ((x)==(y)) tet_result(TET_PASS);  else tet_result(TET_FAIL)
#define TET_CHECK_NOT_EQUAL(x,y) if ((x)!=(y)) tet_result(TET_PASS);  else tet_result(TET_FAIL)

#define TET_CHECK_MESSAGE(x,y) if ((y)==(TRUE)) tet_result(TET_PASS); else {tet_result(TET_FAIL); tet_printf(x);}
#define TET_CHECK_EQUAL_MESSAGE(x,y,z) if ((y)==(z)) tet_result(TET_PASS);  else {tet_result(TET_FAIL); tet_printf(x);}
#define TET_CHECK_NOT_EQUAL_MESSAGE(x,y,z) if ((y)!=(z)) tet_result(TET_PASS);  else {tet_result(TET_FAIL); tet_printf(x);}

#endif


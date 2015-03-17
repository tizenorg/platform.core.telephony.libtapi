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

#ifndef __TAPI_LOG_H__
#define __TAPI_LOG_H__

__BEGIN_DECLS

#ifdef FEATURE_DLOG_DEBUG

#include <dlog.h>

#ifndef TAPI_LOG_TAG
#define TAPI_LOG_TAG "LIBTAPI"
#endif

#define msg(fmt,args...)  { RLOG(LOG_INFO, TAPI_LOG_TAG, fmt "\n", ##args); }
#define dbg(fmt,args...)  { RLOG(LOG_DEBUG, TAPI_LOG_TAG, fmt "\n", ##args); }
#define warn(fmt,args...)  { RLOG(LOG_WARN, TAPI_LOG_TAG, fmt "\n", ##args); }
#define err(fmt,args...)  { RLOG(LOG_ERROR, TAPI_LOG_TAG, fmt "\n", ##args); }
#define fatal(fmt,args...)  { RLOG(LOG_FATAL, TAPI_LOG_TAG, fmt "\n", ##args); }

#define smsg(fmt,args...)  { SECURE_RLOG(LOG_INFO, TAPI_LOG_TAG, fmt "\n", ##args); }
#define sdbg(fmt,args...)  { SECURE_RLOG(LOG_DEBUG, TAPI_LOG_TAG, fmt "\n", ##args); }
#define swarn(fmt,args...)  { SECURE_RLOG(LOG_WARN, TAPI_LOG_TAG, fmt "\n", ##args); }
#define serr(fmt,args...)  { SECURE_RLOG(LOG_ERROR, TAPI_LOG_TAG, fmt "\n", ##args); }
#define sfatal(fmt,args...)  { SECURE_RLOG(LOG_FATAL, TAPI_LOG_TAG, fmt "\n", ##args); }

#else

#define msg(fmt,args...)
#define dbg(fmt,args...)
#define warn(fmt,args...)
#define err(fmt,args...)
#define fatal(fmt,args...)

#define smsg(fmt,args...)
#define sdbg(fmt,args...)
#define swarn(fmt,args...)
#define serr(fmt,args...)
#define sfatal(fmt,args...)

#endif

__END_DECLS

#endif

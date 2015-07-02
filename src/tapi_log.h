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

#define msg(fmt,args...)  do { RLOG(LOG_INFO, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define dbg(fmt,args...)  do { RLOG(LOG_DEBUG, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define warn(fmt,args...)  do { RLOG(LOG_WARN, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define err(fmt,args...)  do { RLOG(LOG_ERROR, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define fatal(fmt,args...)  do { RLOG(LOG_FATAL, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)

#define smsg(fmt,args...)  do { SECURE_RLOG(LOG_INFO, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define sdbg(fmt,args...)  do { SECURE_RLOG(LOG_DEBUG, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define swarn(fmt,args...)  do { SECURE_RLOG(LOG_WARN, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define serr(fmt,args...)  do { SECURE_RLOG(LOG_ERROR, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)
#define sfatal(fmt,args...)  do { SECURE_RLOG(LOG_FATAL, TAPI_LOG_TAG, fmt "\n", ##args); } while (0)

#else

#define msg(fmt,args...) do {} while (0)
#define dbg(fmt,args...) do {} while (0)
#define warn(fmt,args...) do {} while (0)
#define err(fmt,args...) do {} while (0)
#define fatal(fmt,args...) do {} while (0)

#define smsg(fmt,args...) do {} while (0)
#define sdbg(fmt,args...) do {} while (0)
#define swarn(fmt,args...) do {} while (0)
#define serr(fmt,args...) do {} while (0)
#define sfatal(fmt,args...) do {} while (0)

#endif

__END_DECLS

#endif

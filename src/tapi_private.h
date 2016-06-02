/*
 * libtapi
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __TAPI_PRIVATE_H__
#define __TAPI_PRIVATE_H__

__BEGIN_DECLS

#include <system_info.h>

#define TELEPHONY_FEATURE "http://tizen.org/feature/network.telephony"

#define TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(feature_name) { \
	bool telephony_supported = FALSE; \
	if (!system_info_get_platform_bool(feature_name, &telephony_supported)) { \
		if (telephony_supported == FALSE) { \
			err("telephony feature is disabled"); \
			return TAPI_API_NOT_SUPPORTED; \
		} \
	} else { \
		err("Error - Feature getting from System Info"); \
		return TAPI_API_OPERATION_FAILED; \
	} \
}

#define TAPI_RET_ERR_NULL_IF_NOT_SUPPORTED(feature_name) { \
	bool telephony_supported = FALSE; \
	if (!system_info_get_platform_bool(feature_name, &telephony_supported)) { \
		if (telephony_supported == FALSE) { \
			err("telephony feature is disabled"); \
			return NULL; \
		} \
	} else { \
		err("Error - Feature getting from System Info"); \
		return NULL; \
	} \
}

__END_DECLS

#endif /* __TAPI_PRIVATE_H__ */

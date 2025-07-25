/**
 * @brief Mock for DevJSON protocol header file
 * @addtogroup k_devjson_protocol
 * @{
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/* Include -------------------------------------------------------------------*/
#include "fff.h"
#include "k_devjson_protocol.h"

/* Macro ---------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/
/* Constant ------------------------------------------------------------------*/
/* Variable ------------------------------------------------------------------*/
/* Function Declaration ------------------------------------------------------*/
DECLARE_FAKE_VOID_FUNC(k_devjson_protocol_register_callback, k_devjson_protocol_callback_t)
DECLARE_FAKE_VALUE_FUNC(k_devjson_protocol_parse_status_t, k_devjson_protocol_parse, const char *, char *, size_t)
DECLARE_FAKE_VOID_FUNC(k_devjson_protocol_add_response, cJSON *, const char *, k_devjson_protocol_value_t, k_devjson_protocol_value_type_t)

#ifdef __cplusplus
}
#endif
/* @} */
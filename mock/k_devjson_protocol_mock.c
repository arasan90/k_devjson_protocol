/**
 * @file k_devjson_protocol_mock.c
 * @ingroup k_devjson_protocol
 * @{
 */

/* Include -------------------------------------------------------------------*/
#include "k_devjson_protocol_mock.h"

/* Macro ---------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/
/* Function Declaration ------------------------------------------------------*/
/* Constant ------------------------------------------------------------------*/
/* Variable ------------------------------------------------------------------*/
/* Function Definition -------------------------------------------------------*/
DEFINE_FAKE_VOID_FUNC(k_devjson_protocol_register_callback, k_devjson_protocol_callback_t)
DEFINE_FAKE_VALUE_FUNC(k_devjson_protocol_parse_status_t, k_devjson_protocol_parse, const char *, char *, size_t)
DEFINE_FAKE_VOID_FUNC(k_devjson_protocol_add_response, cJSON *, const char *, k_devjson_protocol_value_t, k_devjson_protocol_value_type_t)
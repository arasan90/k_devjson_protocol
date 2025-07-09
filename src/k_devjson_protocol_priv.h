/**
 * @brief Private header file for the DevJSON protocol
 * @addtogroup k_devjson_protocol
 * @{
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/* Include -------------------------------------------------------------------*/
#include "cJSON.h"
#include "k_devjson_protocol.h"

/* Macro ---------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/
/* Constant ------------------------------------------------------------------*/
/* Variable ------------------------------------------------------------------*/
/* Function Declaration ------------------------------------------------------*/

/**
 * @brief Get the ID from a JSON object
 * @param json Pointer to the cJSON object
 * @return The ID value if present, -1 otherwise
 */
int k_devjson_protocol_get_id(const cJSON *json);

/**
 * @brief Return a group type from a JSON object
 * @param json Pointer to the cJSON object
 * @param group_type The type of group to return. Refer to \ref k_devjson_protocol_group_type_t for possible values
 * @return The group type as a cJSON object if present, NULL otherwise
 */
cJSON *k_devjson_protocol_get_group(const cJSON *json, k_devjson_protocol_group_type_t group_type);

void k_devjson_protocol_process_group_entries(int id, cJSON *output_json, const cJSON *group, k_devjson_protocol_group_type_t group_type);

#ifdef __cplusplus
}
#endif
/* @} */
/**
 * @file k_devjson_protocol.c
 * @ingroup k_devjson_protocol
 * @{
 */

/* Include -------------------------------------------------------------------*/
#include <string.h>

#include "k_devjson_protocol_priv.h"

/* Macro ---------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/
/* Function Declaration ------------------------------------------------------*/
/* Constant ------------------------------------------------------------------*/
const char *k_devjson_protocol_id_key = "id";  //!< Key for the ID in DevJSON protocol

const char *k_devjson_protocol_req_key = "req";	 //!< Key for the request in DevJSON protocol
const char *k_devjson_protocol_res_key = "res";	 //!< Key for the response in DevJSON protocol

const char *k_devjson_protocol_get_key = "get";	 //!< Key for the GET group in DevJSON protocol
const char *k_devjson_protocol_set_key = "set";	 //!< Key for the SET group in DevJSON protocol
const char *k_devjson_protocol_cmd_key = "cmd";	 //!< Key for the CMD group in DevJSON protocol

/* Variable ------------------------------------------------------------------*/
k_devjson_protocol_callback_t k_devjson_protocol_callback = NULL;  //!< Global callback function for DevJSON protocol

/* Function Definition -------------------------------------------------------*/
void k_devjson_protocol_register_callback(k_devjson_protocol_callback_t callback)
{
	k_devjson_protocol_callback = callback;	 //!< Register the callback function
}

k_devjson_protocol_parse_status_t k_devjson_protocol_parse(const char *json_string, char *output_string, const size_t output_string_size)
{
	k_devjson_protocol_parse_status_t parse_status	= K_DEVJSON_PROTOCOL_PARSE_ERROR;
	cJSON							 *output_json	= cJSON_CreateObject();
	int								  is_id_correct = 1;
	if (k_devjson_protocol_callback)
	{
		parse_status = K_DEVJSON_PROTOCOL_PARSE_INVALID_JSON;
		if (json_string)
		{
			cJSON *json = cJSON_Parse(json_string);
			if (json)
			{
				int id = k_devjson_protocol_get_id(json);
				if (-1 != id)
				{
					k_devjson_protocol_cb_arg_t id_cb_arg = {.group_type = K_DEVJSON_PROTOCOL_GROUP_TYPE_ID, .id = id};
					k_devjson_protocol_callback(&id_cb_arg);
					if (id == id_cb_arg.id)
					{
						cJSON_AddNumberToObject(output_json, k_devjson_protocol_id_key, id);  //!< Add the ID to the output JSON
					}
					else
					{
						parse_status  = K_DEVJSON_PROTOCOL_PARSE_WRONG_ID;
						is_id_correct = 0;	//!< ID is not correct, set flag to false
					}
				}
				if (is_id_correct)
				{
					cJSON *request_json = k_devjson_protocol_extract_request(json);
					if (request_json)
					{
						cJSON *res_output_json = cJSON_AddObjectToObject(output_json, k_devjson_protocol_res_key);
						{
							cJSON *get_group = k_devjson_protocol_get_group(request_json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
							cJSON *set_group = k_devjson_protocol_get_group(request_json, K_DEVJSON_PROTOCOL_GROUP_TYPE_SET);
							cJSON *cmd_group = k_devjson_protocol_get_group(request_json, K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD);
							if (get_group)
							{
								k_devjson_protocol_process_group_entries(id, res_output_json, get_group, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
							}
							if (set_group)
							{
								k_devjson_protocol_process_group_entries(id, res_output_json, set_group, K_DEVJSON_PROTOCOL_GROUP_TYPE_SET);
							}
							if (cmd_group)
							{
								k_devjson_protocol_process_group_entries(id, res_output_json, cmd_group, K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD);
							}
						}
					}
					parse_status = K_DEVJSON_PROTOCOL_PARSE_SUCCESS;
				}
			}
			cJSON_PrintPreallocated(output_json, output_string, output_string_size, 0);
			cJSON_Delete(output_json);	//!< Clean up the output JSON object
			cJSON_Delete(json);
		}
	}
	return parse_status;
}

int k_devjson_protocol_get_id(const cJSON *json)
{
	int id = -1;
	if (cJSON_HasObjectItem(json, k_devjson_protocol_id_key) && cJSON_IsNumber(cJSON_GetObjectItem(json, k_devjson_protocol_id_key)))
	{
		id = cJSON_GetObjectItem(json, k_devjson_protocol_id_key)->valueint;  //!< Get the ID value from the JSON object
	}
	return id;
}

cJSON *k_devjson_protocol_extract_request(const cJSON *json)
{
	cJSON *request_json = NULL;
	if (cJSON_HasObjectItem(json, k_devjson_protocol_req_key))
	{
		request_json = cJSON_GetObjectItem(json, k_devjson_protocol_req_key);
	}
	return request_json;
}

cJSON *k_devjson_protocol_get_group(const cJSON *json, k_devjson_protocol_group_type_t group_type)
{
	cJSON	   *group_type_json = NULL;
	const char *group_type_key	= NULL;
	switch (group_type)
	{
		case K_DEVJSON_PROTOCOL_GROUP_TYPE_GET:
			group_type_key = k_devjson_protocol_get_key;
			break;
		case K_DEVJSON_PROTOCOL_GROUP_TYPE_SET:
			group_type_key = k_devjson_protocol_set_key;
			break;
		case K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD:
			group_type_key = k_devjson_protocol_cmd_key;
			break;
		default:
			break;
	}
	if (group_type_key)
	{
		if (cJSON_HasObjectItem(json, group_type_key))
		{
			group_type_json = cJSON_GetObjectItem(json, group_type_key);
		}
	}
	return group_type_json;
}

void k_devjson_protocol_process_group_entries(int id, cJSON *output_json, const cJSON *group, k_devjson_protocol_group_type_t group_type)
{
	(void)output_json;
	k_devjson_protocol_cb_arg_t cb_arg = {0};
	cJSON *group_type_response_json	   = K_DEVJSON_PROTOCOL_GROUP_TYPE_GET == group_type ? cJSON_AddObjectToObject(output_json, k_devjson_protocol_get_key) :
										 K_DEVJSON_PROTOCOL_GROUP_TYPE_SET == group_type ? cJSON_AddObjectToObject(output_json, k_devjson_protocol_set_key) :
										 K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD == group_type ? cJSON_AddObjectToObject(output_json, k_devjson_protocol_cmd_key) :
																						   NULL;
	if (group_type_response_json)
	{
		cb_arg.group_type  = group_type;
		cb_arg.id		   = id;
		cb_arg.output_json = group_type_response_json;
		switch (group_type)
		{
			case K_DEVJSON_PROTOCOL_GROUP_TYPE_GET:
				if (cJSON_IsString(group))
				{
					/* Special case */
					cb_arg.key						= group->valuestring;
					cb_arg.input_value.string_value = group->valuestring;
					cb_arg.input_value_type			= K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING;
					k_devjson_protocol_callback(&cb_arg);
				}
				else if (cJSON_IsArray(group))
				{
					cJSON *current_item = group->child;
					while (current_item)
					{
						cb_arg.key = current_item->valuestring;
						if (cJSON_IsString(current_item))
						{
							cb_arg.input_value.string_value = current_item->valuestring;
							cb_arg.input_value_type			= K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING;
						}
						else if (cJSON_IsNumber(current_item))
						{
							cb_arg.input_value.float_value = (float)current_item->valuedouble;
							cb_arg.input_value_type		   = K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT;
						}
						else
						{
							cb_arg.input_value_type = K_DEVJSON_PROTOCOL_VALUE_TYPE_UNKNOWN;
						}
						k_devjson_protocol_callback(&cb_arg);
						current_item = current_item->next;	//!< Move to the next item in the group
					}
				}
				break;
			case K_DEVJSON_PROTOCOL_GROUP_TYPE_SET:
				if (cJSON_IsObject(group))
				{
					cJSON *current_item = group->child;
					while (current_item)
					{
						cb_arg.key = current_item->string;
						if (cJSON_IsString(current_item))
						{
							cb_arg.input_value.string_value = current_item->valuestring;
							cb_arg.input_value_type			= K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING;
						}
						else if (cJSON_IsNumber(current_item))
						{
							cb_arg.input_value.float_value = (float)current_item->valuedouble;
							cb_arg.input_value_type		   = K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT;
						}
						else
						{
							cb_arg.input_value_type = K_DEVJSON_PROTOCOL_VALUE_TYPE_UNKNOWN;
						}
						k_devjson_protocol_callback(&cb_arg);
						current_item = current_item->next;	//!< Move to the next item in the group
					}
				}
				break;
			case K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD:
				if (cJSON_IsObject(group))
				{
					cJSON *current_item = group->child;
					while (current_item)
					{
						cb_arg.key = current_item->string;
						if (cJSON_IsString(current_item))
						{
							cb_arg.input_value.string_value = current_item->valuestring;
							cb_arg.input_value_type			= K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING;
						}
						else if (cJSON_IsNumber(current_item))
						{
							cb_arg.input_value.float_value = (float)current_item->valuedouble;
							cb_arg.input_value_type		   = K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT;
						}
						else if (cJSON_IsBool(current_item))
						{
							cb_arg.input_value.bool_value = cJSON_IsTrue(current_item) ? 1 : 0;	 //!< Convert cJSON boolean to integer
							cb_arg.input_value_type		  = K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL;
						}
						else if (cJSON_IsObject(current_item))
						{
							cb_arg.input_value.json_value = current_item;  //!< Convert cJSON boolean to integer
							cb_arg.input_value_type		  = K_DEVJSON_PROTOCOL_VALUE_TYPE_JSON;
						}
						else
						{
							cb_arg.input_value_type = K_DEVJSON_PROTOCOL_VALUE_TYPE_UNKNOWN;
						}
						k_devjson_protocol_callback(&cb_arg);
						current_item = current_item->next;	//!< Move to the next item in the group
					}
				}
				break;
			default:
				break;
		}
	}
}

void k_devjson_protocol_add_response(cJSON *output_json, const char *key, k_devjson_protocol_value_t value, k_devjson_protocol_value_type_t value_type)
{
	if (output_json && key)
	{
		switch (value_type)
		{
			case K_DEVJSON_PROTOCOL_VALUE_TYPE_INTEGER:
				cJSON_AddNumberToObject(output_json, key, value.int_value);
				break;
			case K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT:
				cJSON_AddNumberToObject(output_json, key, value.float_value);
				break;
			case K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING:
				cJSON_AddStringToObject(output_json, key, value.string_value);
				break;
			case K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL:
				cJSON_AddBoolToObject(output_json, key, value.bool_value);
				break;
			default:
				cJSON_AddNullToObject(output_json, key);  //!< Add null if the output value type is unknown
				break;
		}
	}
}
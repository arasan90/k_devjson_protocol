/**
 * @brief DevJSON protocol header file
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

/* Macro ---------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/
/**
 * @brief DevJSON protocol key value types
 */
typedef enum
{
	K_DEVJSON_PROTOCOL_VALUE_TYPE_INTEGER,	//!< Integer value
	K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT,	//!< Float value
	K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING,	//!< String value
	K_DEVJSON_PROTOCOL_VALUE_TYPE_JSON,		//!< JSON value
	K_DEVJSON_PROTOCOL_VALUE_TYPE_UNKNOWN	//!< Unknown value
} k_devjson_protocol_value_type_t;

/**
 * @brief DevJSON protocol group types
 */
typedef enum
{
	K_DEVJSON_PROTOCOL_GROUP_TYPE_GET,	//<!< Group for GET requests
	K_DEVJSON_PROTOCOL_GROUP_TYPE_SET,	//!< Group for SET requests
	K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD,	//!< Group for CMD requests
} k_devjson_protocol_group_type_t;

typedef union
{
	char  *string_value;  //!< String value
	int	   int_value;	  //!< Integer value
	float  float_value;	  //!< Float value
	cJSON *json_value;	  //!< JSON value (cJSON object)
} k_devjson_protocol_value_t;

/**
 * @brief DevJSON protocol callback argument structure
 */
typedef struct
{
	const char					   *key;				//!< Key of the JSON object
	cJSON						   *output_json;		//!< Output JSON object to which the response will be added
	k_devjson_protocol_value_t		input_value;		//!< Input value to be processed
	k_devjson_protocol_value_t		output_value;		//!< Output value after processing
	int								id;					//!< Unique identifier for the recipient. Optional. -1 if not present
	k_devjson_protocol_group_type_t group_type;			//!< Type of the group (GET, SET, CMD, ID)
	k_devjson_protocol_value_type_t input_value_type;	//!< Type of the input value
	k_devjson_protocol_value_type_t output_value_type;	//!< Type of the output value
} k_devjson_protocol_cb_arg_t;

/**
 * @brief DevJSON protocol parse status
 */
typedef enum
{
	K_DEVJSON_PROTOCOL_PARSE_SUCCESS,	   //!< Parsing was successful
	K_DEVJSON_PROTOCOL_PARSE_ERROR,		   //!< Error occurred during parsing
	K_DEVJSON_PROTOCOL_PARSE_INVALID_JSON  //!< Invalid JSON format
} k_devjson_protocol_parse_status_t;

/**
 * @brief Callback function type for DevJSON protocol
 *
 * This function is called when a JSON object is parsed.
 * It allows processing of the key-value pairs in the JSON object.
 *
 * @param cb_arg Pointer to the callback argument structure containing
 *               information about the parsed JSON object.
 */
typedef void (*k_devjson_protocol_callback_t)(k_devjson_protocol_cb_arg_t *cb_arg);

/* Constant ------------------------------------------------------------------*/
/* Variable ------------------------------------------------------------------*/
/* Function Declaration ------------------------------------------------------*/
/**
 * @brief Register a callback function for DevJSON protocol
 *
 * This function registers a callback that will be called when a JSON object
 * is parsed. The callback can process the key-value pairs in the JSON object.
 *
 * @param callback Pointer to the callback function to be registered.
 */
void k_devjson_protocol_register_callback(k_devjson_protocol_callback_t callback);

/**
 * @brief Parse a JSON string and process it using the registered callback
 *
 * This function parses the provided JSON string and calls the registered
 * callback for each key-value pair in the JSON object.
 *
 * @param json_string Pointer to the JSON string to be parsed.
 * @return k_devjson_protocol_parse_status_t Status of the parsing operation.
 */
k_devjson_protocol_parse_status_t k_devjson_protocol_parse(const char *json_string);

void k_devjson_protocol_add_response(cJSON *output_json, const char *key, k_devjson_protocol_value_t value, k_devjson_protocol_value_type_t value_type);

#ifdef __cplusplus
}
#endif
/* @} */
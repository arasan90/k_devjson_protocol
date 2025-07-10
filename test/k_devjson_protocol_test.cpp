#include "k_devjson_protocol.h"

#include <gtest/gtest.h>

#include "cJSON.h"
#include "k_devjson_protocol_priv.h"

const char *k_devjson_protocol_test_key1_value		 = "test1";
const char *k_devjson_protocol_test_key2_value		 = "test2";
const int	k_devjson_protocol_test_key3_int_value	 = 42;
const float k_devjson_protocol_test_key4_float_value = 2.5f;

void k_devjson_protocol_callback(k_devjson_protocol_cb_arg_t *cb_arg)
{
	switch (cb_arg->group_type)
	{
		case K_DEVJSON_PROTOCOL_GROUP_TYPE_ID:
			if (123 == cb_arg->id)
			{
				cb_arg->id = cb_arg->id;
			}
			else
			{
				cb_arg->id = -1;  //!< Wrong ID
			}
			break;
		case K_DEVJSON_PROTOCOL_GROUP_TYPE_GET:
			if (0 == strcmp(cb_arg->key, "key1"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key,
												(k_devjson_protocol_value_t){.string_value = (char *)k_devjson_protocol_test_key1_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING);
			}
			else if (0 == strcmp(cb_arg->key, "key2"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key,
												(k_devjson_protocol_value_t){.string_value = (char *)k_devjson_protocol_test_key2_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING);
			}
			else if (0 == strcmp(cb_arg->key, "key3"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key,
												(k_devjson_protocol_value_t){.int_value = k_devjson_protocol_test_key3_int_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_INTEGER);
			}
			else if (0 == strcmp(cb_arg->key, "key4"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key,
												(k_devjson_protocol_value_t){.float_value = k_devjson_protocol_test_key4_float_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT);
			}
			else if (0 == strcmp(cb_arg->key, "all"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, "key1",
												(k_devjson_protocol_value_t){.string_value = (char *)k_devjson_protocol_test_key1_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING);
				k_devjson_protocol_add_response(cb_arg->output_json, "key2",
												(k_devjson_protocol_value_t){.string_value = (char *)k_devjson_protocol_test_key2_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_STRING);
				k_devjson_protocol_add_response(cb_arg->output_json, "key3", (k_devjson_protocol_value_t){.int_value = k_devjson_protocol_test_key3_int_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_INTEGER);
				k_devjson_protocol_add_response(cb_arg->output_json, "key4",
												(k_devjson_protocol_value_t){.float_value = k_devjson_protocol_test_key4_float_value},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT);
			}
			break;
		case K_DEVJSON_PROTOCOL_GROUP_TYPE_SET:
			if (0 == strcmp(cb_arg->key, "key1"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key,
												(k_devjson_protocol_value_t){.string_value = (char *)cb_arg->input_value.string_value},
												cb_arg->input_value_type);
			}
			else if (0 == strcmp(cb_arg->key, "key2"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key,
												(k_devjson_protocol_value_t){.string_value = (char *)cb_arg->input_value.string_value},
												cb_arg->input_value_type);
			}
			else if (0 == strcmp(cb_arg->key, "key3"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.int_value = cb_arg->input_value.int_value},
												cb_arg->input_value_type);
			}
			else if (0 == strcmp(cb_arg->key, "key4"))
			{
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.float_value = cb_arg->input_value.float_value},
												cb_arg->input_value_type);
			}
			break;
		case K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD:
			if (0 == strcmp(cb_arg->key, "c1"))
			{
				if (K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL == cb_arg->input_value_type && 1 == cb_arg->input_value.bool_value)
				{
					k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.bool_value = 1},
													K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL);
				}
				else
				{
					// If the input value is not a boolean, we can still add it as a string response
					k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.bool_value = 0},
													K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL);
				}
			}
			else if (0 == strcmp(cb_arg->key, "c2"))
			{
				cJSON *ssid_json	 = cJSON_GetObjectItem(cb_arg->input_value.json_value, "ssid");
				cJSON *password_json = cJSON_GetObjectItem(cb_arg->input_value.json_value, "password");
				if (ssid_json && password_json)
				{
					k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.bool_value = 1},
													K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL);
				}
				else
				{
					k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.bool_value = 0},
													K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL);
				}
			}
			else
			{
				// Handle other commands or unknown commands
				k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.bool_value = 0},
												K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL);
			}
		default:
			break;
	}
}

TEST(KDevJsonProtocol, IDPresent)
{
	std::string json_string = R"({"id": 123, "req":{"get": ["key1","key2"]}})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	EXPECT_EQ(k_devjson_protocol_get_id(json), 123);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, IDNotPresent)
{
	std::string json_string = R"("req":{"get": ["key1","key2"]})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	EXPECT_EQ(k_devjson_protocol_get_id(json), -1);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, GetGroupPresent)
{
	std::string json_string = R"({"get": ["key1","key2"]})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *group		= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	EXPECT_NE(group, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, GetGroups)
{
	std::string json_string = R"({"get": ["key1","key2"], "set": {"key1": "value1"},  "cmd": {"key1": "value1"}})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *get_group	= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	cJSON	   *set_group	= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_SET);
	cJSON	   *cmd_group	= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD);
	EXPECT_NE(get_group, nullptr);
	EXPECT_NE(set_group, nullptr);
	EXPECT_NE(cmd_group, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, GetMissingGroup)
{
	std::string json_string = R"({"get": ["key1","key2"], "cmd": {"key1": "value1"}})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *get_group	= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	cJSON	   *set_group	= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_SET);
	cJSON	   *cmd_group	= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD);
	EXPECT_NE(get_group, nullptr);
	EXPECT_EQ(set_group, nullptr);
	EXPECT_NE(cmd_group, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, CallbackForArrayGet)
{
	std::string json_string = R"({"get": ["key1","key2", "key4"]})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), out_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	char *output_string = cJSON_PrintUnformatted(out_json);
	EXPECT_STREQ(output_string, R"({"get":{"key1":"test1","key2":"test2","key4":2.5}})");
	cJSON_free(output_string);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}

TEST(KDevJsonProtocol, CallbackForGetAllInArray)
{
	std::string json_string = R"({"get": ["all"]})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), out_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	char *output_string = cJSON_PrintUnformatted(out_json);
	EXPECT_STREQ(output_string, R"({"get":{"key1":"test1","key2":"test2","key3":42,"key4":2.5}})");
	cJSON_free(output_string);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}

TEST(KDevJsonProtocol, CallbackForGetSingleAll)
{
	std::string json_string = R"({"get": "all"})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), out_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	char *output_string = cJSON_PrintUnformatted(out_json);
	EXPECT_STREQ(output_string, R"({"get":{"key1":"test1","key2":"test2","key3":42,"key4":2.5}})");
	cJSON_free(output_string);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}

TEST(KDevJsonProtocol, CallbackForSet)
{
	std::string json_string = R"({"set": {"key1": "new_value1", "key2": "new_value2", "key3": 100, "key4": 2.5}})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), out_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_SET),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_SET);
	char *output_string = cJSON_PrintUnformatted(out_json);
	EXPECT_STREQ(output_string, R"({"set":{"key1":"new_value1","key2":"new_value2","key3":100,"key4":2.5}})");
	cJSON_free(output_string);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}

TEST(KDevJsonProtocol, CallbackForCmd)
{
	std::string json_string = R"({"cmd": {"c1": true, "c2": {"ssid":"test_ssid", "password": "test_password"}, "c3":false, "c4":"unknown command"}})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), out_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD);
	char *output_string = cJSON_PrintUnformatted(out_json);
	EXPECT_STREQ(output_string, R"({"cmd":{"c1":true,"c2":true,"c3":false,"c4":false}})");
	cJSON_free(output_string);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}

TEST(KDevJsonProtocol, ExtractRequestSuccess)
{
	std::string json_string = R"({"id": 123, "req": {"get": ["key1", "key2"]}})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *request		= k_devjson_protocol_extract_request(json);
	EXPECT_NE(request, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, ExtractRequestFailure)
{
	std::string json_string = R"({"id": 123, "seq": {"get": ["key1", "key2"]}})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *request		= k_devjson_protocol_extract_request(json);
	EXPECT_EQ(request, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, ParseSuccessWithID)
{
	std::string json_string = R"({"id": 123, "req":{"get": ["key1", "key2"], "set": {"key1": "value1"}, "cmd": {"c1": true, "c5":{"key1":2}}}})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	char							  output_string[1024];
	k_devjson_protocol_parse_status_t status = k_devjson_protocol_parse(json_string.c_str(), output_string, sizeof(output_string));
	EXPECT_EQ(status, K_DEVJSON_PROTOCOL_PARSE_SUCCESS);
	EXPECT_STREQ(output_string, R"({"id":123,"res":{"get":{"key1":"test1","key2":"test2"},"set":{"key1":"value1"},"cmd":{"c1":true,"c5":false}}})");
}

TEST(KDevJsonProtocol, ParseSuccessWithWrongID)
{
	std::string json_string = R"({"id": 12, "req":{"get": ["key1", "key2"], "set": {"key1": "value1"}, "cmd": {"c1": true, "c5":{"key1":2}}}})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	char							  output_string[1024];
	k_devjson_protocol_parse_status_t status = k_devjson_protocol_parse(json_string.c_str(), output_string, sizeof(output_string));
	EXPECT_EQ(status, K_DEVJSON_PROTOCOL_PARSE_WRONG_ID);
}

TEST(KDevJsonProtocol, ParseSuccessWithNoID)
{
	std::string json_string = R"({"req":{"get": ["key1", "key2"], "set": {"key1": "value1"}, "cmd": {"c1": true, "c5":{"key1":2}}}})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	char							  output_string[1024];
	k_devjson_protocol_parse_status_t status = k_devjson_protocol_parse(json_string.c_str(), output_string, sizeof(output_string));
	EXPECT_EQ(status, K_DEVJSON_PROTOCOL_PARSE_SUCCESS);
	EXPECT_STREQ(output_string, R"({"res":{"get":{"key1":"test1","key2":"test2"},"set":{"key1":"value1"},"cmd":{"c1":true,"c5":false}}})");
}
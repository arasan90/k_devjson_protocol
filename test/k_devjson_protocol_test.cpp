#include "k_devjson_protocol.h"

#include <gtest/gtest.h>

#include "cJSON.h"
#include "k_devjson_protocol_priv.h"

const char *k_devjson_protocol_test_key1_value		 = "test1";
const char *k_devjson_protocol_test_key2_value		 = "test2";
const int	k_devjson_protocol_test_key3_int_value	 = 42;
const float k_devjson_protocol_test_key4_float_value = 3.14f;

void k_devjson_protocol_callback(k_devjson_protocol_cb_arg_t *cb_arg)
{
	if (K_DEVJSON_PROTOCOL_GROUP_TYPE_GET == cb_arg->group_type)
	{
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
			k_devjson_protocol_add_response(cb_arg->output_json, cb_arg->key, (k_devjson_protocol_value_t){.int_value = k_devjson_protocol_test_key3_int_value},
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
			k_devjson_protocol_add_response(cb_arg->output_json, "key4", (k_devjson_protocol_value_t){.float_value = k_devjson_protocol_test_key4_float_value},
											K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT);
		}
	}
}

TEST(KDevJsonProtocol, IDPresent)
{
	std::string json_string = R"({"id": 123, "get": ["key1","key2"]})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	EXPECT_EQ(k_devjson_protocol_get_id(json), 123);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, IDNotPresent)
{
	std::string json_string = R"({"get": ["key1","key2"]})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	EXPECT_EQ(k_devjson_protocol_get_id(json), -1);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, GetGroupPresentWithID)
{
	std::string json_string = R"({"id": 123, "get": ["key1","key2"]})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *group		= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	EXPECT_NE(group, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, GetGroupPresentWithNoID)
{
	std::string json_string = R"({"get": ["key1","key2"]})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *group		= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	EXPECT_NE(group, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, GetGroupNotPresent)
{
	std::string json_string = R"({"id": 123})";
	cJSON	   *json		= cJSON_Parse(json_string.c_str());
	cJSON	   *group		= k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	EXPECT_EQ(group, nullptr);
	cJSON_Delete(json);
}

TEST(KDevJsonProtocol, GetGroups)
{
	std::string json_string = R"({"id": 123, "get": ["key1","key2"], "set": {"key1": "value1"},  "cmd": {"key1": "value1"}})";
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
	std::string json_string = R"({"id": 123, "get": ["key1","key2"], "cmd": {"key1": "value1"}})";
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
	std::string json_string = R"({"id": 123, "get": ["key1","key2","key3", "key4"]})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	cJSON_AddNumberToObject(out_json, "id", k_devjson_protocol_get_id(json));
	cJSON *res_json = cJSON_AddObjectToObject(out_json, "res");
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), res_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}

TEST(KDevJsonProtocol, CallbackForGetAllInArray)
{
	std::string json_string = R"({"id": 123, "get": ["all"]})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	cJSON_AddNumberToObject(out_json, "id", k_devjson_protocol_get_id(json));
	cJSON *res_json = cJSON_AddObjectToObject(out_json, "res");
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), res_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}

TEST(KDevJsonProtocol, CallbackForGetSingleAll)
{
	std::string json_string = R"({"id": 123, "get": "all"})";
	k_devjson_protocol_register_callback(k_devjson_protocol_callback);
	cJSON *json		= cJSON_Parse(json_string.c_str());
	cJSON *out_json = cJSON_CreateObject();
	cJSON_AddNumberToObject(out_json, "id", k_devjson_protocol_get_id(json));
	cJSON *res_json = cJSON_AddObjectToObject(out_json, "res");
	k_devjson_protocol_process_group_entries(k_devjson_protocol_get_id(json), res_json, k_devjson_protocol_get_group(json, K_DEVJSON_PROTOCOL_GROUP_TYPE_GET),
											 K_DEVJSON_PROTOCOL_GROUP_TYPE_GET);
	cJSON_Delete(json);
	cJSON_Delete(out_json);
}
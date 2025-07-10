# k_devjson_protocol

A C library for implementing a JSON-based device communication protocol with support for GET, SET, and CMD operations.

## Overview

The k_devjson_protocol library provides a structured way to handle JSON-based communication with embedded devices or services. It supports three main operation types:

- **GET**: Request data from the device
- **SET**: Configure device parameters
- **CMD**: Execute commands on the device

The protocol includes optional ID validation for request/response matching and supports various data types including strings, integers, floats, booleans, and JSON objects.

## Protocol Structure

### Request Format
```json
{
  "id": 123,                    // Optional: Device identifier
  "req": {                      // Required: Request object
    "get": ["key1", "key2"],    // GET: Array of keys to retrieve
    "set": {                    // SET: Object with key-value pairs
      "key1": "value1",
      "key2": 42
    },
    "cmd": {                    // CMD: Command object
      "command1": true,
      "command2": {"param": "value"}
    }
  }
}
```

### Response Format
```json
{
  "id": 123,                    // Optional: Matches device ID
  "res": {                      // Required: Response object
    "get": {                    // GET responses
      "key1": "value1",
      "key2": "value2"
    },
    "set": {                    // SET confirmations
      "key1": "value1",
      "key2": 42
    },
    "cmd": {                    // CMD results
      "command1": true,
      "command2": false
    }
  }
}
```

## Features

- **Type Safety**: Support for multiple data types (string, integer, float, boolean, JSON)
- **ID Validation**: Optional request/response ID matching
- **Flexible Operations**: GET, SET, and CMD operation types
- **Callback-based**: User-defined callback functions for handling requests
- **Error Handling**: Comprehensive error status reporting
- **Memory Safe**: Built on top of cJSON library

## Building

### For Development and Testing

This project uses CMake for building with development features enabled:

```bash
mkdir build
cd build
cmake -DK_DEVJSON_PROTOCOL_DEV=true ..
make (or appropriate command)
```

### For Integration into Larger Projects

To use this library in your project, include the `k_devjson_protocol.cmake` file in your CMakeLists.txt:

```cmake
# In your CMakeLists.txt
include(path/to/k_devjson_protocol.cmake)

# Get the library components
k_devjson_protocol_get_sources(DEVJSON_SOURCES)
k_devjson_protocol_get_public_headers(DEVJSON_PUBLIC_HEADERS)
k_devjson_protocol_get_private_headers(DEVJSON_PRIVATE_HEADERS)
k_devjson_protocol_get_private_linked_libs(DEVJSON_PRIVATE_LIBS)

# Create your target
add_executable(your_app ${DEVJSON_SOURCES} your_sources.c)
target_include_directories(your_app PUBLIC ${DEVJSON_PUBLIC_HEADERS})
target_include_directories(your_app PRIVATE ${DEVJSON_PRIVATE_HEADERS})
target_link_libraries(your_app PRIVATE ${DEVJSON_PRIVATE_LIBS})

# Don't forget to create the dependency libraries
k_devjson_protocol_create_dep_libraries()
```

## Usage

### Basic Setup

1. **Include the header**:
```c
#include "k_devjson_protocol.h"
```

2. **Implement a callback function**:
```c
void my_callback(k_devjson_protocol_cb_arg_t *cb_arg)
{
    switch (cb_arg->group_type)
    {
        case K_DEVJSON_PROTOCOL_GROUP_TYPE_ID:
            // Handle ID validation
            if (cb_arg->id == EXPECTED_ID) {
                cb_arg->id = cb_arg->id;  // Accept ID
            } else {
                cb_arg->id = -1;  // Reject ID
            }
            break;

        case K_DEVJSON_PROTOCOL_GROUP_TYPE_GET:
            // Handle GET requests
            if (strcmp(cb_arg->key, "temperature") == 0) {
                k_devjson_protocol_add_response(cb_arg->output_json,
                    cb_arg->key,
                    (k_devjson_protocol_value_t){.float_value = 25.5f},
                    K_DEVJSON_PROTOCOL_VALUE_TYPE_FLOAT);
            }
            break;

        case K_DEVJSON_PROTOCOL_GROUP_TYPE_SET:
            // Handle SET requests
            if (strcmp(cb_arg->key, "led_state") == 0) {
                // Process the input value
                bool led_on = cb_arg->input_value.bool_value;
                set_led(led_on);

                // Confirm the setting
                k_devjson_protocol_add_response(cb_arg->output_json,
                    cb_arg->key,
                    cb_arg->input_value,
                    cb_arg->input_value_type);
            }
            break;

        case K_DEVJSON_PROTOCOL_GROUP_TYPE_CMD:
            // Handle CMD requests
            if (strcmp(cb_arg->key, "reset") == 0) {
                bool success = perform_reset();
                k_devjson_protocol_add_response(cb_arg->output_json,
                    cb_arg->key,
                    (k_devjson_protocol_value_t){.bool_value = success},
                    K_DEVJSON_PROTOCOL_VALUE_TYPE_BOOL);
            }
            break;
    }
}
```

3. **Register the callback and parse requests**:
```c
int main() {
    // Register your callback
    k_devjson_protocol_register_callback(my_callback);

    // Parse incoming JSON requests
    const char *request = "{\"id\": 123, \"req\": {\"get\": [\"temperature\"]}}";
    char response[1024];

    k_devjson_protocol_parse_status_t status =
        k_devjson_protocol_parse(request, response, sizeof(response));

    if (status == K_DEVJSON_PROTOCOL_PARSE_SUCCESS) {
        printf("Response: %s\n", response);
        // Output: {"id":123,"res":{"get":{"temperature":25.5}}}
    }

    return 0;
}
```

## Examples

### GET Operation
**Request**:
```json
{"id": 1, "req": {"get": ["sensor1", "sensor2"]}}
```

**Response**:
```json
{"id": 1, "res": {"get": {"sensor1": "active", "sensor2": 42.5}}}
```

### SET Operation
**Request**:
```json
{"id": 2, "req": {"set": {"threshold": 30, "enabled": true}}}
```

**Response**:
```json
{"id": 2, "res": {"set": {"threshold": 30, "enabled": true}}}
```

### CMD Operation
**Request**:
```json
{"id": 3, "req": {"cmd": {"calibrate": true, "reset": {"type": "soft"}}}}
```

**Response**:
```json
{"id": 3, "res": {"cmd": {"calibrate": true, "reset": false}}}
```

### Special GET Cases

**Single string GET**:
```json
{"req": {"get": "all"}}
```

**Array GET**:
```json
{"req": {"get": ["key1", "key2", "key3"]}}
```

## API Reference

### Types

- `k_devjson_protocol_value_type_t`: Enumeration of supported value types
- `k_devjson_protocol_group_type_t`: Enumeration of operation types
- `k_devjson_protocol_parse_status_t`: Parse operation status codes
- `k_devjson_protocol_cb_arg_t`: Callback argument structure

### Functions

- `k_devjson_protocol_register_callback()`: Register a callback function
- `k_devjson_protocol_parse()`: Parse JSON request and generate response
- `k_devjson_protocol_add_response()`: Add response data in callback

### Status Codes

- `K_DEVJSON_PROTOCOL_PARSE_SUCCESS`: Parsing successful
- `K_DEVJSON_PROTOCOL_PARSE_WRONG_ID`: ID validation failed
- `K_DEVJSON_PROTOCOL_PARSE_ERROR`: General parsing error
- `K_DEVJSON_PROTOCOL_PARSE_INVALID_JSON`: Invalid JSON format

## Testing

Run the test suite (requires development build):

```bash
mkdir build
cd build
cmake -DK_DEVJSON_PROTOCOL_DEV=true ..
make (or appropriate command)
ctest
```

The test suite includes comprehensive tests for:
- ID validation
- GET/SET/CMD operations
- Type handling
- Error conditions
- Protocol compliance

## Dependencies

- [cJSON](https://github.com/DaveGamble/cJSON): JSON parsing library
- [GoogleTest](https://github.com/google/googletest): Testing framework (for tests only)

## License

This project is licensed under the terms specified in the LICENSE file.

## Contributing

1. Fork the repository
2. Add tests for new functionality
3. Ensure all tests pass
4. Check for memory leaks using valgrind
5. Submit a pull request

## Error Handling

The library provides comprehensive error handling:

- **Invalid JSON**: Returns `K_DEVJSON_PROTOCOL_PARSE_INVALID_JSON`
- **Wrong ID**: Returns `K_DEVJSON_PROTOCOL_PARSE_WRONG_ID`
- **Missing callback**: Returns `K_DEVJSON_PROTOCOL_PARSE_ERROR`
- **Memory allocation failures**: Handled gracefully

## Performance Considerations

- Use appropriate buffer sizes for response strings
- The library is designed for embedded systems with limited resources
- Callback functions should be efficient to avoid blocking
- Consider using static buffers for better memory management in embedded contexts
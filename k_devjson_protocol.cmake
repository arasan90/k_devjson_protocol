cmake_minimum_required(VERSION 3.10)

set(sources
    ${CMAKE_CURRENT_LIST_DIR}/src/k_devjson_protocol.c
    )

set(public_includes
    ${CMAKE_CURRENT_LIST_DIR}/include
    )

set(private_includes
    ${CMAKE_CURRENT_LIST_DIR}/src
    )

set(public_linked_libs
)

set(private_linked_libs
    k_cjson
    )

function(k_devjson_protocol_get_sources OUT_VAR)
    set(${OUT_VAR}
        ${sources}
        PARENT_SCOPE)
endfunction()

function(k_devjson_protocol_get_public_headers OUT_VAR)
    set(${OUT_VAR}
        ${public_includes}
        PARENT_SCOPE)
endfunction()

function(k_devjson_protocol_get_private_headers OUT_VAR)
    set(${OUT_VAR}
        ${private_includes}
        PARENT_SCOPE)
endfunction()

function(k_devjson_protocol_get_private_linked_libs OUT_VAR)
    set(${OUT_VAR}
        ${private_linked_libs}
        PARENT_SCOPE)
endfunction()

function(k_devjson_protocol_get_public_linked_libs OUT_VAR)
    set(${OUT_VAR}
        ${public_linked_libs}
        PARENT_SCOPE)
endfunction()

function (k_devjson_protocol_create_mock_library)
    add_library(k_devjson_protocol_mock ${CMAKE_CURRENT_LIST_DIR}/mock/k_devjson_protocol_mock.c)
    target_include_directories(k_devjson_protocol_mock PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
    target_include_directories(k_devjson_protocol_mock PRIVATE ${CMAKE_CURRENT_LIST_DIR}/mock)
    target_link_libraries(k_devjson_protocol_mock PUBLIC k_cjson)
endfunction()

function(k_devjson_protocol_create_dep_libraries)
    add_subdirectory(libs/cjson)
endfunction()
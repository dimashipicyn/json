cmake_minimum_required(VERSION 3.0.0)
project(json)

add_executable(json
    main.c
    btree.c
    json.c
)

#set( CMAKE_C_FLAGS "-fsanitize=address" )

#LIBFT
set(FT_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lft/libft.a)
add_custom_target(build_ft ALL 
                   COMMAND ${CMAKE_MAKE_PROGRAM}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lft
                   COMMENT "Original ft makefile target")

add_library(ft STATIC IMPORTED)
set_property(TARGET ft APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ft PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${FT_LIBRARY}")

add_dependencies(ft build_ft)

target_link_libraries(json
    ft
)

target_include_directories(json PUBLIC
    "${PROJECT_SOURCE_DIR}/lft"
)
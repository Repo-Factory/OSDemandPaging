cmake_minimum_required(VERSION 3.5)
project(main_functionality)

get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)

file(GLOB SOURCES 
    ${SRC_DIR}/main_functionality/src/*.cpp
    ${SRC_DIR}/page_table/src/page_table.cpp
    ${SRC_DIR}/page_table/src/page_node.cpp
    ${SRC_DIR}/page_table/src/page_functions.cpp
    ${SRC_DIR}/page_table/src/page_map.cpp
    ${SRC_DIR}/page_replacement/src/*.cpp
    ${SRC_DIR}/helpers/logging/src/*.c
    ${SRC_DIR}/helpers/vr_trace/src/*.c
)

include_directories(
    ${SRC_DIR}/main_functionality/include
    ${SRC_DIR}/page_table/include
    ${SRC_DIR}/page_replacement/include
    ${SRC_DIR}/helpers/logging/include
    ${SRC_DIR}/helpers/vr_trace/include
)

add_library(
    ${PROJECT_NAME}
    SHARED 
    ${SOURCES}
)

add_executable(
    ${PROJECT_NAME}_exec
    main/main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_exec PRIVATE 
    ${PROJECT_NAME}
)

install(
    TARGETS ${PROJECT_NAME} 
    DESTINATION lib/${PROJECT_NAME}
)

install(
    TARGETS ${PROJECT_NAME}_exec 
    DESTINATION bin/${PROJECT_NAME}
)

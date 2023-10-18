cmake_minimum_required(VERSION 3.5)
project(demand_paging)

SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS "-g")

# file(GLOB SOURCES 
#     main_functionality/src/*.cpp
#     page_table/src/page_table.cpp
#     page_table/src/page_node.cpp
#     page_table/src/page_functions.cpp
#     page_table/src/page_map.cpp
#     page_replacement/src/*.cpp
#     helpers/logging/src/*.cpp
#     helpers/vr_trace/src/*.cpp
# )

include_directories(
    main_functionality/include
    page_table/include
    page_replacement/include
    helpers/logging/include
    helpers/vr_trace/include
)

# add_library(
#     ${PROJECT_NAME}
#     SHARED 
# )

add_executable(
    ${PROJECT_NAME}_exec
    main_functionality/src/args_handling.cpp
    page_table/src/page_table.cpp
    page_table/src/page_node.cpp
    page_table/src/page_functions.cpp
    page_table/src/page_map.cpp
    page_replacement/src/wsclock_algorithm.cpp
    helpers/logging/src/log_helpers.cpp
    helpers/vr_trace/src/vaddr_tracereader.cpp
    main_functionality/src/main_functionality.cpp
    main_functionality/main/main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_exec PRIVATE 
    # ${PROJECT_NAME}
)

# install(
#     TARGETS ${PROJECT_NAME} 
#     DESTINATION lib/${PROJECT_NAME}
# )

install(
    TARGETS ${PROJECT_NAME}_exec 
    DESTINATION bin/${PROJECT_NAME}
)


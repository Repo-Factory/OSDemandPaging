cmake_minimum_required(VERSION 3.5)
project(demand_paging)

file(GLOB SOURCES 
    main_functionality/src/*.cpp
    page_table/src/*.cpp
    page_replacement/src/*.cpp
    helpers/bit_operations/src/*.cpp
    helpers/logging/src/*.cpp
    helpers/optional/src/*.cpp
    helpers/vr_trace/src/*.cpp
)

include_directories(
    main_functionality/include
    page_table/include
    page_replacement/include
    helpers/bit_operations/include
    helpers/logging/include
    helpers/optional/include
    helpers/vr_trace/include
)

add_library(
    ${PROJECT_NAME}
    SHARED 
    ${SOURCES}
)

add_executable(
    ${PROJECT_NAME}_exec
    main_functionality/main/main.cpp
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


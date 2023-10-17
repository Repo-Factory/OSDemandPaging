cmake_minimum_required(VERSION 3.5)
project(page_replacement)

get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)

# find_library(LIBRARY library)
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS "-g")
file(GLOB SOURCES 
    ${SRC_DIR}/page_replacement/src/*.cpp
    ${SRC_DIR}/page_table/src/*.cpp
)
include_directories(include
    ${SRC_DIR}/page_table/include
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
    ${PROJECT_NAME} PRIVATE 
    # ${LIBRARY}
)

target_link_libraries(
    ${PROJECT_NAME}_exec PRIVATE 
    ${PROJECT_NAME}
)

install(
    TARGETS ${PROJECT_NAME} 
    DESTINATION lib/${PROJECT_NAME}
)

file(GLOB HEADERS include/*.h)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

install(
    TARGETS ${PROJECT_NAME}_exec 
    DESTINATION bin/${PROJECT_NAME}
)

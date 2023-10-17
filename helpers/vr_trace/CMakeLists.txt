cmake_minimum_required(VERSION 3.5)
project(vr_trace)

get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)

# find_library(LIBRARY library)

file(GLOB SOURCES src/*.c)
include_directories(include)

add_library(
    ${PROJECT_NAME}
    SHARED 
    ${SOURCES}
)

add_executable(
    ${PROJECT_NAME}_exec
    main/main.c
)

target_link_libraries(
    ${PROJECT_NAME}_exec PUBLIC 
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

cmake_minimum_required(VERSION 3.10)

# set the project name
project(Zmanim)

set(SRC
    src/get_zmanim.cpp
    src/main.cpp
    src/parse/parse_date.cpp
    src/time_util.cpp
    src/timespan.cpp
    src/zmanim/day_info.cpp
    src/zmanim/document_info.cpp
    src/zmanim/zman_info.cpp
)

set(INCS
    src
    vendor
    vendor/json/include
)

# add the executable
add_executable(Zmanim ${SRC})

target_include_directories(Zmanim PRIVATE ${INCS})

target_link_libraries(Zmanim ws2_32 crypt32 cryptui ssl crypto)

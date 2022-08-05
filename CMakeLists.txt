cmake_minimum_required(VERSION 3.10)

# set the project name
project(Zmanim)

set(SRC
    src/command/args_util.cpp
    src/command/parse_command.cpp
    src/command/parse_date.cpp
    src/html_builders/day_html.cpp
    src/html_builders/document_html.cpp
    src/html_builders/forecast_entry_html.cpp
    src/html_builders/zman_html.cpp
    src/main.cpp
    src/time_util.cpp
    src/web/forecast.cpp
    src/web/hebrew_dates.cpp
    src/web/zip_codes.cpp
    src/web/zmanim.cpp
)

set(INCS
    src
    vendor
    vendor/cpp-httplib
    vendor/json/include
)

# add the executable
add_executable(Zmanim ${SRC})

target_include_directories(Zmanim PRIVATE ${INCS})

target_link_libraries(Zmanim ws2_32 crypt32 cryptui ssl crypto)

cmake_minimum_required(VERSION 3.16)
project(Bible)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# Include source files
qt_add_executable(Bible
    src/main.cpp
    src/BibleWindow.cpp
    src/BibleDatabase.cpp
)
qt6_add_resources(RESOURCES resources/bible.qrc)

target_sources(Bible PRIVATE
    src/main.cpp
    src/BibleWindow.cpp
    ${RESOURCES}
)




# Link Qt libraries
target_link_libraries(Bible
    PUBLIC
    Qt6::Widgets
    Qt6::Sql
)


# Include directories
target_include_directories(Bible PRIVATE src)
set(CMAKE_PREFIX_PATH "/usr/lib64/cmake/Qt6")


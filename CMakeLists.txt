	
cmake_minimum_required(VERSION 3.1...3.20.0)
project(kosmosoft VERSION 1.0.0)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LOG_LEVEL_BUILD 3)

#Bring the headers
include_directories(ext/ft2/include)
include_directories(ext)
include_directories(include)

#find FreeType library
find_library(FREETYPE_LIB freetype PATHS ${PROJECT_SOURCE_DIR}/ext/ft2/libs/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${BIN_DIR_SUFFIX})
add_executable(kosmosoft 
               ${PROJECT_SOURCE_DIR}/src/font.cpp
               ${PROJECT_SOURCE_DIR}/include/font.h 
               ${PROJECT_SOURCE_DIR}/src/serialize.cpp
               ${PROJECT_SOURCE_DIR}/include/serialize.h 
               ${PROJECT_SOURCE_DIR}/src/geometry.cpp
               ${PROJECT_SOURCE_DIR}/include/geometry.h 
               ${PROJECT_SOURCE_DIR}/src/bezier.cpp
               ${PROJECT_SOURCE_DIR}/include/bezier.h 
               ${PROJECT_SOURCE_DIR}/src/contours.cpp
               ${PROJECT_SOURCE_DIR}/include/contours.h 
               ${PROJECT_SOURCE_DIR}/src/tools.cpp
               ${PROJECT_SOURCE_DIR}/include/tools.h 
               ${PROJECT_SOURCE_DIR}/main.cpp 
               )

target_link_libraries(kosmosoft 
                      PRIVATE
                      ${FREETYPE_LIB} )
target_compile_features(kosmosoft PRIVATE cxx_std_20)
add_custom_command(
    TARGET kosmosoft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/ext/ft2/libs/freetype.dll"
            "$<TARGET_FILE_DIR:kosmosoft>"
)
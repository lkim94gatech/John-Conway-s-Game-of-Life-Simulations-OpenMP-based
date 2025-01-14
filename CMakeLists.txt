# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab2 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(OPENAL_LIBRARY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/libs-msvc/x64/openal32.lib)


# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/codes/*.cpp)

# Add the executable
add_executable(Lab2 ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab2 PUBLIC sfml-graphics sfml-system sfml-window)

#set(OpenMP_CXX_FLAGS "-I/opt/homebrew/Cellar/libomp/19.1.0/include")
#set(OpenMP_CXX_LIBRARIES "/opt/homebrew/Cellar/libomp/19.1.0/lib/libomp.dylib")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Ofast)
target_link_libraries(Lab2 PRIVATE ${OpenMP_CXX_LIBRARIES})
target_compile_options(Lab2 PRIVATE ${OpenMP_CXX_FLAGS})

set_target_properties(
    Lab2 PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)
# Copy DLLs using file(COPY ...)
# file(COPY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/bin/x64/openal32.dll
#      DESTINATION "${COMMON_OUTPUT_DIR}/bin")
# file(COPY ${PROJECT_SOURCE_DIR}/fonts
#      DESTINATION "${COMMON_OUTPUT_DIR}/bin")
# file(COPY ${PROJECT_SOURCE_DIR}/graphics
#      DESTINATION "${COMMON_OUTPUT_DIR}/bin")
#  file(COPY "${PROJECT_SOURCE_DIR}/low res"
#  DESTINATION "${COMMON_OUTPUT_DIR}/bin")
#  file(COPY ${PROJECT_SOURCE_DIR}/sound
#  DESTINATION "${COMMON_OUTPUT_DIR}/bin")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -pthread)

file(GLOB SRCS *.cpp)

# add_library(watchcat STATIC ${SRCS})
add_library(watchcat SHARED ${SRCS})

target_link_libraries(watchcat)

# Specify here the include directories exported by this library
target_include_directories(watchcat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
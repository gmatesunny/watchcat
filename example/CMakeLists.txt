file(GLOB SRCS *.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(example ${SRCS})

# Specify here the libraries this program depends on
target_link_libraries(example watchcat)

install(TARGETS example
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testwatchcat ${SRCS})

TARGET_LINK_LIBRARIES(testwatchcat watchcat libgtest libgmock)

add_test(NAME testwatchcat COMMAND testwatchcat)
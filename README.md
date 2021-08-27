# watchcat: Yet Another Watchdog Library 
An open-source thread-safe C++11 timer exposing APIs in C++, Python2 and Java programming language. It provides a simple to use and easy to understand interface. This project does not depends on any third-party library and uses modern C++.

### Clone the project
    git clone https://github.com/gmatesunny/watchcat.git

### Build the library and google test    
    cd build
    cmake ..
    make

### Build the interface for Python2 and Java  
    cd swig
    bash ./generate.sh

### Note:
It can also be used as a header only library for C++11 without the need to create `SHARED` or `STATIC` library. You only need to include `libwatchcat/timer.hpp` and `libwatchcat/time.cpp` files in your custom project. 

Feel free to open any topic in `Discussion` section of github for this watchcat repository.

### Core Design

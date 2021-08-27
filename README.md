# watchcat: Yet Another Watchdog Library 
An open-source thread-safe C++11 timer exposing APIs in C++, Python2 and Java programming language. It provides a simple to use and easy to understand interface. This project does not depends on any third-party library and uses modern C++.

### Clone the project
    git clone https://github.com/gmatesunny/watchcat.git

### Build the library and google test    
    cd build
    cmake ..
    make

### Build the APIs for Python2 and Java  
    cd swig
    bash ./generate.sh

### Run as follows
    cd build
    ./example/example
    ./test/testwatchcat/testwatchcat


### Ussage:

* Adapt `libwatchcat/CMkakeLists.txt` to build to create `SHARED` or `STATIC` library as per your custom project need and then add `add_subdirectory(libwatchcat)` your root `CMkakeLists.txt` file

* It could be used as a header only library for C++11 without the need to create `SHARED` or `STATIC` library. You only need to include `libwatchcat/timer.hpp` and `libwatchcat/time.cpp` files in your custom project. 

Feel free to open any topic in `Discussion` section of github for this watchcat repository.

### Project Structure

<pre>
├── build
├── CMakeLists.txt
├── example
│   ├── CMakeLists.txt
│   └── main.cpp
├── libwatchcat
│   ├── CMakeLists.txt
│   ├── timer.cpp
│   └── timer.hpp
├── LICENSE
├── README.md
├── swig
│   ├── generate.sh
│   ├── main.java
│   ├── main.py
│   ├── timer.i
└── test
    ├── CMakeLists.txt
    └── testwatchcat
        ├── CMakeLists.txt
        ├── main.cpp
        ├── testwatchcat.cpp
        └── testwatchcat.hpp
</pre>

### Core Design

The followng diagram shows the class UML diagram.

![Class UML](docs/img/classStructure.png)

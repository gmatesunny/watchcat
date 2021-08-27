sudo apt-get install -y swig

swig -c++ -python timer.i
g++ -fPIC -pthread -std=c++11 -I/usr/include/python2.7 -c ../libwatchcat/timer.cpp timer_wrap.cxx
g++ -shared timer.o timer_wrap.o -o _watchcat.so
rm -f timer.o timer_wrap.o timer_wrap.cxx

swig -c++ -java timer.i
# You might have to change the include path; run `update-java-alternatives -l` on linux terminal
g++ -fPIC -pthread -std=c++11 -I/usr/lib/jvm/java-1.11.0-openjdk-amd64/include/ -I/usr/lib/jvm/java-1.11.0-openjdk-amd64/include/linux -c ../libwatchcat/timer.cpp timer_wrap.cxx
g++ -shared timer.o timer_wrap.o -o libwatchcat.so
javac main.java
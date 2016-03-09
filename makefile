
commtcl.dll	:	commtcl.o commbase.o
	g++ -shared -o commtcl.dll commtcl.o commbase.o -lm -ltcl -mwindows -mconsole
commtclelsa.dll	:	commtcl.o commbaseelsa.o
	g++ -shared -o commtclelsa.dll commtcl.o commbase.o -lm -ltcl -mwindows -mconsole
commtcl.o	:	commtcl.cpp commbase.h
	g++  -c commtcl.cpp
commbaseelsa.o	:	commbase.cpp commbase.h
	g++  -c commbase.cpp -DELSA

robosock.exe	:	robotsocket.o robosock.o commbase.o commutil.o commhtml.o
	g++ -o robosock.exe robotsocket.o robosock.o commbase.o commutil.o commhtml.o -lm 

robotsocket.o	:	robotsocket.cpp robosock.h
	g++ -c robotsocket.cpp

robosock.o	:	robosock.cpp robosock.h commbase.h commutil.h commhtml.h
	g++ -c robosock.cpp
commbase.o	:	commbase.cpp commbase.h
	g++  -c commbase.cpp 
commutil.o	:	commutil.cpp commutil.h commbase.h
	g++ -c commutil.cpp
commhtml.o	:	commhtml.cpp commhtml.h commutil.h
	g++ -c commhtml.cpp

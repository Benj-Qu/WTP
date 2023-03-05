all: wSender wReceiver

CXXFLAGS = -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic -O3 -DNDEBUG

wSender: wSender.cpp info.cpp
	g++ $< -o $@ -I $(CXXFLAGS)

wReceiver: wReceiver.cpp
	g++ $< -o $@ -I $(CXXFLAGS)

clean:
	rm -f *.o wSender wReceiver
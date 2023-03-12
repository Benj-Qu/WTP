all: wSender wReceiver

CXXFLAGS = -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic -O3 -DNDEBUG

SenderSource = wSender.cpp utils/*.cpp

ReceiverSource = wReceiver.cpp utils/*.cpp

wSender: $(SenderSource)
	g++ $(SenderSource) -o $@ -I $(CXXFLAGS)

wReceiver: $(ReceiverSource)
	g++ $(ReceiverSource) -o $@ -I $(CXXFLAGS)

clean:
	rm -f *.o wSender wReceiver
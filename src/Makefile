CXX=clang++
WARNINGS=-Wall -Wextra -Wconversion -Wshadow -Weffc++
CXXFLAGS=${WARNINGS} -std=c++14 -O0 -ggdb
INCLUDES=-isystem ../extern/Beast/include/ -isystem ../extern/json/src/
LIBS=-lncurses -lboost_system -lboost_program_options -lpthread -lcryptopp
TEST_LIBS=-lgtest -lgtest_main

UI_OBJECTS=user_interface.o ncurses_interface.o game_screen.o ncurses_window.o message_window.o board_window.o web_interface.o websocket_connection.o
CRYPTO_OBJECTS=commitment.o
NETWORK_OBJECTS=tcp_connection.o
GAME_OBJECTS=game.o board.o game_communicator.o
OBJECTS=options.o util.o ${GAME_OBJECTS} ${UI_OBJECTS} ${CRYPTO_OBJECTS} ${NETWORK_OBJECTS}
TEST_OBJECTS=dummy_connection.o test_dummy_connection.cpp test_board.o test_commitment.o test_util.o

BINARIES=battleship test test_web test_ws test_tcp game_screen

.phony: all clean


all: ${BINARIES}

battleship: battleship.o ${OBJECTS}
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS}

game_screen: test_game_screen.o ${UI_OBJECTS} ${GAME_OBJECTS}
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS}

test_tcp: test_tcp.o game_communicator.o tcp_connection.o
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS}

test_web: test_web.o ${UI_OBJECTS} board.o
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS}

test_ws: test_ws.o websocket_connection.o
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS}

test: test.o ${OBJECTS} ${TEST_OBJECTS}
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS} ${TEST_LIBS}

runtest: test
	./test

game_communicator.o: game_communicator.cpp message_generated.h
	${CXX} ${CXXFLAGS} -c $<

%.o: %.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $^

%_generated.h: %.fbs
	flatc --cpp --scoped-enums $^

clean:
	rm -f *.o
	rm -f ${BINARIES}
	rm -f *_generated.h

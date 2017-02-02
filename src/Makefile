CXX=clang++
WARNINGS=-Wall -Wextra
CXXFLAGS=${WARNINGS} -std=c++14 -O0 -ggdb
LIBS=-lncurses -lboost_system -lpthread -lcryptopp
TEST_LIBS=-lgtest -lgtest_main

UI_OBJECTS=user_interface.o ncurses_interface.o config_screen.o game_screen.o ncurses_window.o message_window.o board_window.o
CRYPTO_OBJECTS=commitment.o
GAME_OBJECTS=game.o board.o tcp_connection.o
OBJECTS=util.o ${GAME_OBJECTS} ${UI_OBJECTS} ${CRYPTO_OBJECTS}
TEST_OBJECTS=test_board.o test_commitment.o test_util.o

.phony: clean commitment


all: battleship test

battleship: battleship.o ${OBJECTS}
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS}

game_screen: test_game_screen.o ${UI_OBJECTS}
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS}

test: test.o ${OBJECTS} ${TEST_OBJECTS}
	${CXX} -o $@ ${CXXFLAGS} $^ ${LIBS} ${TEST_LIBS}

runtest: test
	./test

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $^

clean:
	rm -f *.o
	rm -f test battleship game_screen

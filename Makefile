#If you want to use clang or some other compiler, change this. Must be in your $PATH
CC=g++ # Compiler

#If you need more source files, 
SRCS=$(wildcard src/*.cpp)
INCLUDE=include
HEADERS=$(wildcard $(INCLUDE)/*.hpp)
TEST_INCLUDE=test/include
TEST_HEADERS=$(wildcard $(TEST_INCLUDE)/*.hpp)
TEST_SRCS=$(wildcard test/src/*.cpp)

# Transform x.cpp -> x.o
OBJ=$(addsuffix .o, $(basename $(SRCS)))
TEST_OBJ=$(addsuffix .o, $(basename $(TEST_SRCS)))

# The name of your executable
BIN=example
TEST_BIN=tests
CPPFLAGS=-g

all: $(BIN) test

test: $(TEST_BIN)
	./$(TEST_BIN)

Main.o: Main.cpp $(HEADERS)
	$(CC) -I$(INCLUDE) $(CPPFLAGS) -c -o $@ $<

src/%.o: src/%.cpp $(HEADERS)
	$(CC) -I$(INCLUDE) $(CPPFLAGS) -c -o $@ $<

TestMain.o: TestMain.cpp $(HEADERS) $(TEST_HEADERS)
	$(CC) -I$(INCLUDE) -I$(TEST_INCLUDE) $(CPPFLAGS) -c -o $@ $<

test/src/%.o: test/src/%.cpp $(HEADERS) $(TEST_HEADERS)
	$(CC) -I$(INCLUDE) -I$(TEST_INCLUDE) $(CPPFLAGS) -c -o $@ $<

$(BIN): $(OBJ) Main.o
	$(CC) -o $(BIN) $(OBJ) Main.o

$(TEST_BIN): $(OBJ) $(TEST_OBJ) $(HEADERS) $(TEST_HEADERS) TestMain.o
	$(CC) -o $(TEST_BIN) $(OBJ) $(TEST_OBJ) TestMain.o

clean:
	-rm $(OBJ)
	-rm $(BIN)
	-rm $(TEST_OBJ)
	-rm $(TEST_BIN)
	-rm Main.o
	-rm TestMain.o
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
BIN=myshell
TEST_BIN=tests
CPPFLAGS=-g

all: $(BIN) $(TEST_BIN)
	./$(TEST_BIN)

.cpp.o: $(TEST_SRCS) $(SRCS) $(HEADERS)
	$(CC) -I$(INCLUDE) -I$(TEST_INCLUDE) $(CPPFLAGS) -c -o $@ $<

$(BIN): $(OBJ) main.o
	$(CC) -o $(BIN) $(OBJ) main.o

$(TEST_BIN): $(OBJ) $(TEST_OBJ) $(HEADERS) $(TEST_HEADERS) testMain.o
	$(CC) -o $(TEST_BIN) $(OBJ) $(TEST_OBJ) testMain.o

clean:
	-rm $(OBJ)
	-rm $(BIN)
	-rm $(TEST_OBJ)
	-rm $(TEST_BIN)
	-rm main.o
	-rm testMain.o
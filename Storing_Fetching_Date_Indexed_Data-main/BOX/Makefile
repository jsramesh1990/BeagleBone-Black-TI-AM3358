CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
SRC = src/main.cpp src/date.cpp src/box.cpp
OBJ = $(SRC:.cpp=.o)
BIN = bin/boxes

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC)

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: all clean

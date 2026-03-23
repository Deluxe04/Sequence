# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
TARGET = sequence_test

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

HEADERS = exceptions.h \
          dynamic_array.h \
          linked_list.h \
          sequence.h \
          array_sequence.h \
          list_sequence.h \
          bit_sequence.h

all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o


clean:
	rm -f $(TARGET) $(OBJS)


run: $(TARGET)
	./$(TARGET)

# Отладка
debug: CXXFLAGS += -DDEBUG -g
debug: $(TARGET)

.PHONY: all clean run debug
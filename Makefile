CXX ?= g++
CXXFLAGS ?= -Wall -O3
SRCS = src/*.cpp
HDRS = src/*.h
TARGET = simulation

$(TARGET): $(SRCS) $(HDRS)
	$(CXX) -std=c++14 $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

run:
	./$(TARGET)

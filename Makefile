# Compiler and flags
CXX = g++
CXXFLAGS = -g -O2 -std=c++17 -Wall
LDFLAGS = -lsimlib -lm

# Source files
SRCS = ims.cpp
HEADERS = ims.hpp
OBJS = $(SRCS:.cpp=.o)

# Output binary
TARGET = pvc

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compilation
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

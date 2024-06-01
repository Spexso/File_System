# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Executable file
EXECUTABLE = $(BIN_DIR)/my_program.exe

# Default target
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Run the executable
run: $(EXECUTABLE)
	@echo "Block size (KB):" $(BLOCK_SIZE)
	@echo "File name:" $(FILE_NAME)
	$(EXECUTABLE) $(BLOCK_SIZE) $(FILE_NAME)

# Clean up the build
clean:
	if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR)
	if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)

# Phony targets
.PHONY: all clean run

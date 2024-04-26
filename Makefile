CC := clang++
CXXFLAGS := -Wall $(shell llvm-config --cxxflags)
LDFLAGS := $(shell llvm-config --ldflags)
LIBS := $(shell llvm-config --libs)

BIN_DIR := bin

TARGET := $(BIN_DIR)/cfg
SRCS := src/execPath.cpp src/cfg.cpp src/main.cpp

$(TARGET): $(SRCS) | $(BIN_DIR)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: clean
clean:
	rm -f $(TARGET)

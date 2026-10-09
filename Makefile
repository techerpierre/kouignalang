BUILD_DIR = .dist
TARGET = $(BUILD_DIR)/main
SOURCE = src/*.cpp \
	src/core/*.cpp \
	src/lexer/*.cpp \
	src/ast/*.cpp \
	src/parser/*.cpp \
	src/interpreter/*.cpp \
	src/sema/*.cpp \
	src/transpiler/*.cpp \
	src/commands/*.cpp \
	src/utils/*.cpp

IN_DEV_FILE = $(BUILD_DIR)/input.kl
OUT_DEV_FILE = $(BUILD_DIR)/output.cpp

.PHONY:
	build
	create-in
	run
	run-in
	run-inout

build:
	mkdir -p .dist
	g++ $(SOURCE) -o $(TARGET)

create-in:
	touch $(IN_DEV_FILE)

run:
	./$(TARGET)

run-in:
	./$(TARGET) -i $(IN_DEV_FILE)

run-inout:
	./$(TARGET) -i $(IN_DEV_FILE) -o $(OUT_DEV_FILE)

run-ast:
	./$(TARGET) -i $(IN_DEV_FILE) -p

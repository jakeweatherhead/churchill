.PHONY: all build clean run

BUILDDIR = build
DATADIR = data
EXECUTABLE = ./build/churchill

all: build

build:
	@echo "Building churchill..."
	@mkdir -p build data
	@cd build && cmake .. && cmake --build .
	@echo "Executing churchill..."
	@$(EXECUTABLE)

c:
	@rm -rf build data

v:
	valgrind --leak-check=full --track-origins=yes $(EXECUTABLE)

callgrind:
	valgrind --tool=callgrind $(EXECUTABLE)

db:
	docker build -t jakeweatherhead/churchill .
.PHONY: all build clean run

# Project configuration
BUILDDIR = build
EXECUTABLE = ./$(BUILDDIR)/churchill

all: build

build:
	@mkdir -p $(BUILDDIR)
	@cd $(BUILDDIR) && cmake .. && cmake --build .

c:
	@rm -rf $(BUILDDIR)

run:
	@$(EXECUTABLE)
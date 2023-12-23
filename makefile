.PHONY: all build clean run

# Project configuration
BUILDDIR = build
DATADIR = data
EXECUTABLE = ./$(BUILDDIR)/churchill

all: build

build:
	@mkdir -p $(BUILDDIR) $(DATADIR)
	@cd $(BUILDDIR) && cmake .. && cmake --build .

c:
	@rm -rf $(BUILDDIR) $(DATADIR)

run:
	@$(EXECUTABLE)
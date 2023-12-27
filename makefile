.PHONY: all build clean run

BUILDDIR = build
DATADIR = data
EXECUTABLE = ./$(BUILDDIR)/churchill

all: build

build:
	@mkdir -p $(BUILDDIR) $(DATADIR)
	@cd $(BUILDDIR) && cmake .. && cmake --build .

c:
	@rm -rf $(BUILDDIR) $(DATADIR)

db:
	docker build -t jakeweatherhead/churchill .

run:
	@$(EXECUTABLE)
.PHONY: all build clean run c v profile kcachegrind db

EXECUTABLE = ./build/churchill

all: build

build:
	@mkdir -p build data/pcp_0/res data/pcp_0/arb data/pcp_1/res data/pcp_1/arb
	@cd build && cmake .. && cmake --build .

run:
	@$(EXECUTABLE)

c:
	@rm -rf build data cachegrind.log cachegrind.out.* callgrind.log callgrind.out.*

v:
	valgrind --leak-check=full --track-origins=yes $(EXECUTABLE) > valgrind.log 2>&1

profile: profile_callgrind profile_cachegrind

profile_callgrind:
	valgrind --tool=callgrind $(EXECUTABLE) > callgrind.log 2>&1

profile_cachegrind:
	valgrind --tool=cachegrind $(EXECUTABLE) > cachegrind.log 2>&1

kcachegrind:
	eval $$(dbus-launch --sh-syntax) && kcachegrind callgrind.out.*

db:
	docker build -t jakeweatherhead/churchill .

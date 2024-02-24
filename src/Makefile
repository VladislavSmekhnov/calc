CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++17
TFLAGS = -fprofile-arcs -ftest-coverage -lgtest
ADDFLAGS = -lm -lrt -lsubunit -lpthread
OS := $(shell uname -s)
BUILD_DIR = build
MCFILES = $(shell find model -name "*.cc")
TCFILES = $(shell find tests -name "*.cc")

ifeq ($(OS), Linux)
	TFLAGS += -lm -lrt -lsubunit
	QMAKECMD = qmake6
	OPENCMD = xdg-open
	DVIOPENCMD = code
else
	QMAKECMD = qmake
	OPENCMD = open
	DVIOPENCMD = open
endif

.PHONY: all dvi clean style dist install test run style-replace uninstall add_coverage_flag gcov_report

all: dvi clean style dist install gcov_report run

style-replace:
	clang-format --style=Google -i $(shell find . -name "*.cc" -o -name "*.h")

style: style-replace
	clang-format --style=Google -n $(shell find . -name "*.cc" -o -name "*.h")

uninstall:
	$(shell find ./$(BUILD_DIR) ! -name '.gitkeep' -type f -exec rm -f {} +)
	$(shell find ./$(BUILD_DIR) ! -name '$(BUILD_DIR)' -type d -exec rm -rf {} +)

test:
	$(CC) $(CFLAGS) $(ADDFLAGS) $(MCFILES) $(TCFILES) -o test $(TFLAGS)
ifeq ($(OS), Linux)
	valgrind --tool=memcheck --leak-check=yes ./test
else
	leaks -atExit -- ./test | grep LEAK:
endif

gcov_report: add_coverage_flag test
	gcov $(MCFILES)
#	mv *.gcov ./gcov/
	mv *.gc* ./gcov/
	lcov -t gcov/ -o report.info -c -d . --no-external
	genhtml -o report report.info
	$(OPENCMD) ./report/index.html

add_coverage_flag:
	$(eval ADDFLAGS += --coverage)
	@echo s21_smart_calc_v2.0: Check build

run:
ifeq ($(OS), Linux)
	./$(BUILD_DIR)/smart_calc
else
	open $(BUILD_DIR)/smart_calc.app
endif

dvi:
	$(DVIOPENCMD) dvi.md

install:
	cd view && $(QMAKECMD) smart_calc.pro && make && make clean && rm Makefile
ifeq ($(OS), Darwin)
	mv view/smart_calc.app $(BUILD_DIR)
else
	mv view/smart_calc $(BUILD_DIR)
endif

dist: clean
	tar -czf s21_smart_calc.tar.gz model view controller Makefile

clean:
	rm -rf test $(shell find . -name "*.o") gcov/*.gcno gcov/*.gcda *.info report s21_smart_calc.tar.gz

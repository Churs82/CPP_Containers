# Linux, Darwin or Windows_NT
OS=$(shell uname -s)
CC := g++
FLAGS := -I inc -std=c++17 -Wall -Wextra -g -lstdc++#-Wpedantic -Werror -lasan -fsanitize=address -fsanitize=leak
CFLAGS:=$(FLAGS) -c -x c++ 
LFLAGS = 
TSTFLAGS = -lgtest -lgtest_main 
SHELL = /bin/sh
DIR_BUILD=$(shell pwd)/
DIR_INSTALL=~/
DIR_TESTS=tests/
DIR_IF=./
DIR_LIB=./
TEST_SRC=$(wildcard tests/*.cpp)
LIB_SRC=$(wildcard *.cpp) # https://runebook.dev/ru/docs/gnu_make/wildcard-function
LIB_HEADERS=$(wildcard *.h)
TEST_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(TEST_SRC:.cpp=.o)))
LIB_OBJS:=$(addprefix $(DIR_BUILD),$(notdir $(LIB_SRC:.cpp=.o)))
LIB = s21_containers.a
LIB_NAME = s21_containers

OS_MAC_UX=
RM:=-rm -rf

.phony: all, clean, dist, test, gcov_report, $(LIB), gcov_flag, lib, test_nl, control, cppcheck, clang, check_format, leak_check  

all: clang clean $(LIB) test 
	
clean:
	$(RM) $(DIR_BUILD)report/* $(DIR_BUILD)*.o $(DIR_BUILD)*.info $(DIR_BUILD)*.gcov $(DIR_BUILD)*.gcda $(DIR_BUILD)*.gcno $(DIR_BUILD)$(LIB) $(DIR_BUILD)$(LIB_NAME)_test

dist: $(LIB)
	echo tar-`sed \
	-e '/version_string/!d' \
	-e 's/[^0-9.]*\([0-9.]*\).*/\1/' \
	-e q version.c` > .fname
	-rm -rf `cat .fname`
	mkdir `cat .fname`
	ln $(DIR_BUILD)$(LIB) `cat .fname`
	tar chzf `cat .fname`.tar.gz `cat .fname`
	-rm -rf `cat .fname` .fname

test: test_nl
	$(DIR_BUILD)$(LIB_NAME)_test

gcov_report: clean gcov_flag $(LIB) test
	-lcov -t "$(LIB_NAME)" -o $(DIR_BUILD)$(LIB_NAME)_report.info -c -d $(DIR_BUILD) --no-external --exclude "$(DIR_BUILD)tests/*" --rc lcov_branch_coverage=1
#	-geninfo  $(DIR_BUILD) -o $(DIR_BUILD)$(LIB_NAME)_report.info --no-external --exclude "$(DIR_BUILD)tests/*" --rc lcov_branch_coverage=1
	-genhtml -o $(DIR_BUILD)report $(DIR_BUILD)$(LIB_NAME)_report.info --branch-coverage
	-mkdir $(DIR_BUILD)report
	cd $(DIR_BUILD)
	-@python3 -m gcovr -r . --html --html-details -o $(DIR_BUILD)report/index.html --exclude tests
ifeq ($(OS), Windows_NT) 
	start $(DIR_BUILD)report/index.html
else
	open $(DIR_BUILD)report/index.html
endif

$(LIB): $(DIR_BUILD)$(LIB)

$(DIR_BUILD)$(LIB): $(LIB_OBJS) $(LIB_HEADERS)
	ar r $@ $(LIB_OBJS)
	ranlib $@

$(LIB_OBJS): $(LIB_SRC) $(LIB_HEADERS)
	$(CC) $(CFLAGS) $(DIR_LIB)$(patsubst %.o,%.cpp,$(notdir $@)) $(GCOV) -o $(DIR_BUILD)$(notdir $@)

$(TEST_OBJS): $(TEST_SRC)
	$(CC) $(CFLAGS) $(DIR_TESTS)$(patsubst %.o,%.cpp,$(notdir $@)) $(GCOV) -o $(DIR_BUILD)$(notdir $@) 

lib: $(LIB)

test_nl: $(LIB) $(TEST_OBJS)

ifeq ($(OS),Darwin)
	$(eval OS_MAC_UX=-L/opt/homebrew/lib)
endif
	$(CC) $(FLAGS) $(GCOV) $(TEST_OBJS) -o $(DIR_BUILD)$(LIB_NAME)_test $(LFLAGS) -l:$(LIB) $(TSTFLAGS) $(OS_MAC_UX) -L. -L$(DIR_BUILD)

gcov_flag:
#	add parametrs
	$(eval GCOV=-DGCOV -fprofile-arcs -ftest-coverage -coverage -fno-inline -fno-inline-small-functions -fno-default-inline) 

leak_check: test_nl
ifeq ($(OS),Darwin)
	leaks -atExit -- $(DIR_BUILD)$(LIB_NAME)_test
else
	-valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s $(DIR_BUILD)$(LIB_NAME)_test
endif
	
control: clang leak_check cppcheck
	
cppcheck:
	-cppcheck --std=c++17 --language=c++ --enable=all --suppress=missingIncludeSystem --error-exitcode=1 *.cpp *.h *.hpp *.tpp tests/*.cpp tests/*.h tests/*.tpp tests/*.hpp

clang:
	find . -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.tpp" -o -name "*.c" \) -exec clang-format-18 -i -style=file:../materials/linters/.clang-format {} \;

check_format: 
	find . -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.tpp" -o -name "*.c" \) -exec clang-format-18 -n -style=file:../materials/linters/.clang-format {} \;
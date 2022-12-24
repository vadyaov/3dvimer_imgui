CC := gcc
vimerlib := vimer.a
vimerlib_gcov := gcov$(vimerlib)

CFLAGS := -Wall -Werror -Wextra -std=c11 -pedantic -fanalyzer
TEST_FLAGS := $(CFLAGS) $(shell pkg-config --cflags check)
GCOV_FLAGS := -fprofile-arcs -ftest-coverage

TST_LIBS ?= $(shell pkg-config --libs check) 
ifeq ($(shell uname), Linux)
	TST_LIBS = $(shell pkg-config --libs check) -lcheck_pic -lpthread -lrt -lsubunit -lm
endif

SRC_FILES := $(shell find . -maxdepth 1 -name "*.c")
SRC_OBJS := $(addprefix ./objects/, $(notdir $(SRC_FILES:.c=.o)))
SRC_INC := $(shell find . -maxdepth 1 -name "*.h")

TEST_FILES := $(shell find ./tests -maxdepth 1 -name "*_test.c")
TEST_OBJS := $(addprefix ./tests/objects/, $(notdir $(TEST_FILES:.c=.o)))
TEST_INC := ./tests/test.h
TEST_MAIN := ./tests/test.c
TEST_MAIN_OBJ := ./tests/objects/test.o
TEST_ALL_OBJS := $(TEST_OBJS) $(TEST_MAIN_OBJS)

# put this objs in other dir because they include coverage flags
GCOV_OBJS := $(addprefix ./tests/objects/, $(notdir $(SRC_FILES:.c=.o)))

GCOV_MAIN_FILE := ./coverage
GCOV_INFO_FILE := coverage.info

GCDA := $(shell find ./tests/objects -maxdepth 1 -name "*.gcda")
GCNO := $(shell find ./tests/objects -maxdepth 1 -name "*.gcno")

all: $(vimerlib) test #gcov_report

$(vimerlib): $(SRC_OBJS) $(SRC_INC)
	ar rc $@ $(SRC_OBJS)
	ranlib $@
	
$(vimerlib_gcov): $(GCOV_OBJS) $(SRC_INC)
	ar rc $@ $(GCOV_OBJS)
	ranlib $@

$(SRC_OBJS): ./objects%.o: .%.c $(SRC_INC)
	@mkdir -p objects
	$(CC) $(CFLAGS) -o $@ -c $<

$(GCOV_OBJS): ./tests/objects%.o: .%.c $(SRC_INC)
	@mkdir -p tests/objects
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $@ -c $<

test: $(vimerlib) $(TEST_OBJS) $(TEST_MAIN_OBJ) $(TEST_INC)
	$(CC) $(TEST_OBJS) $(TEST_MAIN_OBJ) -o tests/test $(TST_LIBS) -L. $(vimerlib) #-fsanitize=address -g
	./tests/test

$(TEST_OBJS): ./tests/objects%.o: ./tests%.c $(TEST_INC)
	@mkdir -p ./tests/objects
	$(CC) $(TEST_FLAGS) -o $@ -c $<

$(TEST_MAIN_OBJ): $(TEST_MAIN)
	@mkdir -p ./tests/objects
	$(CC) $(CFLAGS) -o $@ -c $<

gcov_report: $(GCOV_MAIN_FILE)
	open coverage/index.html

$(GCOV_MAIN_FILE): $(GCOV_INFO_FILE)
	genhtml $< -o $@

$(GCOV_INFO_FILE): gcov_test
	./tests/test_gcov
	geninfo ./tests/objects -b . -o ./$@

gcov_test: $(vimerlib_gcov) $(TEST_MAIN_OBJ) $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(TEST_MAIN_OBJ) $(GCOV_FLAGS) -o tests/test_gcov $(TST_LIBS) -L. $<

clean:
	@rm -f $(SRC_OBJS) 
	@rm -f $(TEST_ALL_OBJS) 
	@rm -f $(vimerlib) $(vimerlib_gcov) 
	@rm -f coverage.info
	@rm -f $(GCDA) $(GCNO)
	@rm -f ./tests/test
	@rm -f ./tests/test_gcov
	@rm -rf ./objects/ ./tests/objects/ ./coverage/

valgrind:
	valgrind --leak-check=full ./tests/test

cppcheck:
	cppcheck --enable=all $(wildcard *.c)

clang:
	clang-format -n $(wildcard *.c) $(wildcard *.h)

.PHONY: clean all valgrind cppcheck clang test

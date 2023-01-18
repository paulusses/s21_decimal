CC=gcc -std=c11 -D_GNU_SOURCE
CFLAGS= -Wall -Werror -Wextra
CFLAGS_GCOV= --coverage

SOURCES = arithmetics.c bigdecfuncs.c comparsion.c converters.c extrafuncs.c otherfuncs.c
SOURCE_LIB = s21_decimal.a

SOURCES_TEST = tests1.c
OUTFILE_TEST = s21_decimal_test

BUILD_DIR = .

OS=$(shell uname)
ifeq ($(OS), Linux)
CFLAGS_TEST= -lcheck -lm -lpthread -lrt -lsubunit
LEAK_CHECK = valgrind --leak-check=full -s
endif
ifeq ($(OS), Darwin)
CFLAGS_TEST= -lcheck
LEAK_CHECK = leaks -q --atExit -- 
endif

all: clean $(SOURCE_LIB)

$(SOURCE_LIB): s21_dec.o
	@ar -rc $(SOURCE_LIB) *.o
	@ranlib $(SOURCE_LIB)
	@rm *.o

test: clean 
	$(CC) $(CFLAGS) $(SOURCES) $(SOURCES_TEST) $(CFLAGS_TEST) -o $(OUTFILE_TEST)
	./$(OUTFILE_TEST)

s21_dec.o: s21_decimal.h
	@$(CC) $(CFLAGS) -c $(SOURCES)

leaks: test
	$(LEAK_CHECK) ./$(OUTFILE_TEST)

check:
	cp ../materials/linters/.clang-format ../
	clang-format -n ../src/*.c
	clang-format -n ../src/*.h

gcov_report: clean $(SOURCE_LIB)
	gcc --coverage $(SOURCES) $(SOURCES_TEST) -o s21_test -lcheck 
	./s21_test
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info

clean:
	rm -rf ./lcov_report
	rm -rf *.o *.a *.gcno *.out *.dSYM *.gcda *.gcov *.info ./$(OUTFILE_TEST) *.txt s21_test ./gcov_tests report

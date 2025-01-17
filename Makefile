CC = gcc
CPP = g++
ANALYSIS_FLAGS = -g3 -Wall -Werror -std=c17 -pedantic
DEBUG_FLAGS = -I/usr/lib -L/usr/lib -lgtest -pthread -g3 -fno-omit-frame-pointer -fsanitize=address -Wall -Werror -std=c17 -pedantic
TESTING_FLAGS = -I/usr/lib -L/usr/lib -lgtest -pthread -g3 -fno-omit-frame-pointer -fsanitize=address -std=c17
DEPLOY_FLAGS = -Wall -Werror -std=c17 -pedantic -O2

analysis_build: main.c record.c cli.c
	$(CC) $(ANALYSIS_FLAGS) main.c record.c cli.c -o $@

debug_build: main.c record.c cli.c
	$(CPP) main.c record.c cli.c -o $@ $(DEBUG_FLAGS)

test_build: main.c record.c cli.c
	$(CPP) main.c record.c cli.c -o $@ $(TESTING_FLAGS)

deploy_build: main.c record.c cli.c
	$(CC) $(DEPLOY_FLAGS_FLAGS) main.c record.c cli.c -o $@

clean:
	rm -f analysis_build debug_build test_build deploy_build

.PHONY:
	clean
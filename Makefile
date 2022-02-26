CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3
OFLAGS = -O3 -march=native -mtune=native -fdelete-null-pointer-checks -finline-functions -funroll-loops -faggressive-loop-optimizations -floop-interchange -floop-unroll-and-jam -fpeel-loops -fsplit-loops -ftree-loop-vectorize -ftree-slp-vectorize
FILES = src/*.c
AR = THEOPHILE_MOLINATTI

# Rules
all: dfa_des

dfa_des: $(FILES)
	$(CC) $(CFLAGS) $(OFLAGS) $(FILES) -o $@

run: dfa_des
	@./dfa_des

archive:
	@mkdir -p $(AR)
	@cp -r src/ Makefile README.md $(AR)
	@tar zcvf $(AR).tar.gz $(AR)
	@rm -Rf $(AR)

clean:
	@rm -Rf dfa_des $(AR).tar.gz
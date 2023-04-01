CC = gcc -O2 -ftree-vectorize -Wall -Wextra -Wpedantic -Wvla -I"include/"

all: lib.a tests.exe bench.exe demo.exe

lib.a: simd/f32.o tensor.o print.o ops.o

tests.exe: tests.o lib.a
bench.exe: bench.o lib.a
demo.exe: demo.o lib.a

%.a: 
	@echo " [AR]" $@
	@gcc-ar rcs $@ $^

# All linkage
%.exe:
	@echo " [LD]" $@
	@$(CC) $^ -o $@

# All .c files
%.o: %.c
	@echo " [CC]" $<
	@$(CC) -c -o $@ $<

clean:
	rm -f *.o *.a *.exe

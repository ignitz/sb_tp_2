Sources 	= ./src
Builds 		= ./build

all: clean mkdirs util.o
	gcc -o tp2 $(Sources)/assembly.c $(Builds)/util.o

clean:
	rm -rf $(Builds)
	rm -f tp2

util.o:
	gcc -c $(Sources)/util.c -o $(Builds)/util.o

mkdirs:
	mkdir $(Builds)

run:
	./tp2 tst/tp2ex1_fatorial.asm
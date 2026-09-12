all: bin
	gcc src/*.c -Iinc/ -o bin/resistor_divider -lm

test: test/test.c
	gcc test/test.c src/calc_tools.c src/standard_resistors.c -Iinc/ -lm -o test/runtest

bin:
	mkdir bin

clean:
	rm bin/*

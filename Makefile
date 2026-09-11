all: bin
	gcc src/*.c -Iinc/ -o bin/resistor_divider -lm

test: bin
	gcc src/test.c src/calc_tools.c src/standard_resistors.c -Iinc/ -lm

bin:
	mkdir bin

clean:
	rm bin/*

all: bin
	gcc src/*.c -Iinc/ -o bin/resistor_divider -lm

x86_64:
	x86_64-w64-mingw32-gcc src/*.c -Iinc/ -o bin/resistor_divider.exe -lm

test: test/test.c
	gcc test/test.c src/calc_tools.c src/standard_resistors.c -Iinc/ -lm -o bin/test

bin:
	mkdir bin

clean:
	rm bin/*

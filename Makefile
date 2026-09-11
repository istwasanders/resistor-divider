all: bin
	gcc src/*.c -Iinc/ -o bin/resistor_divider -lm

bin:
	mkdir bin

clean:
	rm bin/*

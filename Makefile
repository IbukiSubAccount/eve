exec = eve.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	bash ./src/shell/run.sh
	cp ./eve.out /usr/local/bin/eve
	bash ./src/shell/version.sh

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
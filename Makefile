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
	cp ./eve.out /usr/local/bin/eve
	@echo '================================================================='
	@echo 'Version: 1.4.1'

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
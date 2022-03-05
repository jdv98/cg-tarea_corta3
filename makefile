CFLAGS=-Wall `pkg-config --cflags --libs MagickWand`

main: main.o
	gcc main.o $(CFLAGS) -o main

main.o: main.h

clean:
	rm -f main
	rm -f *.o
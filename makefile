CFLAGS=`pkg-config --cflags --libs MagickWand`

main: main.o
	gcc main.o $(CFLAGS) -o main

main.o: main.h

valgrind: clean main
	valgrind ./main $(file_name) $(width) $(height) $(background) $(line) $(sx) $(sy) $(ex) $(ey) $(sw)

clean:
	rm -f main
	rm -f *.o

#make file_name="prueba.png" width="20" height="20" background="black" line="red" sx="0" sy="0" ex="5" ey="5" sw="2" debug
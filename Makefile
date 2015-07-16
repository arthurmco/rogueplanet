CC=gcc
CFLAGS = -lncurses -g -Wall

rogueplanet: main.o colony.o planet.o structures.o
	$(CC) -o $@ $^ $(CFLAGS)
	
main.o: src/main.c
	$(CC) -o $@ -c src/main.c $(CFLAGS)
	
colony.o: src/colony.c
	$(CC) -o  $@ -c src/colony.c $(CFLAGS)
	
planet.o: src/planet.c
	$(CC) -o $@ -c src/planet.c $(CFLAGS)
	
structures.o: src/structures.c
	$(CC) -o $@ -c src/structures.c $(CFLAGS)
	
clean:
	rm *.o

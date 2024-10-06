CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -pedantic -g

# Build the main dictionary executable
lzw: encodetests.o decodetests.o encode.o decode.o dict.o
	$(CC) $(CFLAGS) -o lzw encodetests.o encode.o decodetests.o decode.o dict.o

debug: encodetests.o decodetests.o encode.o decode.o dict.o
	$(CC) $(CFLAGS) -o lzw encodetests.o encode.o decodetests.o decode.o dict.o

encodetests.o: encodetests.c encode.h
	$(CC) $(CFLAGS) -c encodetests.c 

encode.o: encode.c encode.h 
	$(CC) $(CFLAGS) -c encode.c 

decodetests.o: decodetests.c decode.h
	$(CC) $(CFLAGS) -c decodetests.c 

decode.o: decode.c decode.h
	$(CC) $(CFLAGS) -c decode.c 

clean:
	rm lzw encodetests.o encode.o decodetests.o decode.o dict.o

# Run Valgrind to check for memory leaks
grind: lzw
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./lzw -x out1.bin tmp.txt

grinde: lzw
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./lzw -c in1.txt tmp.bin


gdb: lzw
	gdb ./lzw

test: lzw
	./lzw in1.txt
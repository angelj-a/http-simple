CC = gcc
CFLAGS = -Wall -g
OBJS = http.o parser.o main.o yr.o

main: $(OBJS)
	@ $(CC) $(CFLAGS) $^ -o  $@
	@ echo Compilado con éxito
	@ echo Ejecute: ./main

%.o: %.c
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@ rm -f ./*.o
	@ rm -f main
	@ rm -f a.out
	@ clear
	@ echo Listo

new: clean main

all:
	${CC} -Wall -Iinc -Llib -lcmark -o mdoc mdoc.c

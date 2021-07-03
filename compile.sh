gcc -c mappa.c -lm
gcc -c entity.c -lm
gcc -c classifica.c -lm
gcc -c main.c -lm
gcc -o  gioco mappa.o entity.o classifica.o main.o -lm

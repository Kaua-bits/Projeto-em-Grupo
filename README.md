projeto em grupo sobre o jogo em C da AT1 N2 de Algoritmos e Programação Estruturada, feito por: Kauã Téo e Gabriel Henrique.

1-Primeiro compila o arquivo .c da biblioteca seguindo essa lógica:
gcc -c bib.c -o bib.o

2-Depois cria um arquivo de biblioteca com o comando
ar rcs libbib.a bib.o

3-Aí depois vai ser o
gcc Jogo.c -L. -lbib -o Jogo

4-E depois executar com
./Jogo










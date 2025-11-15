# Projeto-em-Grupo
projeto em grupo sobre o jogo em C da AT1 N2 de Algoritmos e Programação Estruturada, feito por: Kauã Téo e Gabriel Henrique.

Primeiro compila o arquivo .c da biblioteca seguindo essa lógica:
gcc -c bib.c -o bib.o

Depois cria um arquivo de biblioteca com o comando
ar rcs libbib.a bib.o

Aí depois vai ser o
gcc Jogo.c -L. -lbib -o Jogo

E depois executar com
./Jogo










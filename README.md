projeto em grupo sobre o jogo em C da AT1 N2 de Algoritmos e Programação Estruturada, feito por: Kauã Téo Dos Santos e Gabriel Henrique Cruz da Silva.

1-Primeiro compila o arquivo .c da biblioteca seguindo essa lógica:
gcc -c bib.c -o bib.o

2-Depois cria um arquivo de biblioteca com o comando:
ar rcs libbib.a bib.o

3-Agora compila o arquivo "main" do jogo junto com a biblioteca, usando o comando:
gcc Jogo.c -L. -lbib -o Jogo

4-E depois executar com:
./Jogo










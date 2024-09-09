1. Criei os arquivos da biblioteca (.h) e implementação (.c) e o código principal.

2. Compilei o arquivo de implementação (.c) da biblioteca.

	-- $ gcc -c absolute.c -o absolutelibobject.o

3. Compilei a biblioteca para criar um arquivo de objeto (.o).

	-- $ ar rcs libabsolute.a absolutelibobject.o

4. Compilei o programa principal e vinculei a biblioteca estática.

	-- $ gcc pratica1_main.c -L. -labsolute -o pratica1_executable

5. Executei o programa
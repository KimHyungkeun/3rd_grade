ssu_prog : ssu_main.o ssu_os.o ssu_esp.o ssu_lsp.o
	gcc -Wall -W ssu_main.o ssu_os.o ssu_esp.o ssu_lsp.o -o ssu_prog

ssu_main.o : ssu_main.c
	gcc -Wall -W -c ssu_main.c

ssu_os.o : ssu_os.c
	gcc -Wall -W -c ssu_os.c


ssu_esp.o : ssu_esp.c
	gcc -Wall -W -c ssu_esp.c


ssu_lsp.o : ssu_lsp.c
	gcc -Wall -W -c ssu_lsp.c

nfshell: shell.c
	gcc shell.c -o nfshell

run: nfshell
	./nfshell

clean:
	rm *~
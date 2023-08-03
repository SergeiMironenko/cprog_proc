Server=server
Client=client
FUN=erproc.c

make: clean server client

server: $(Server).c
	gcc -o $(Server) $(Server).c $(FUN) -pthread

client: $(Client).c
	gcc -o $(Client) $(Client).c $(FUN) -pthread

clean:
	rm -rf $(Server) $(Client)

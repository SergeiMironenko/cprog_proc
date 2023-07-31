Server=server
Client=client
FUN=fun

make: clean server client

server: $(Server).c
	gcc -o $(Server) $(Server).c

client: $(Client).c
	gcc -o $(Client) $(Client).c

clean:
	rm -rf $(Server) $(Client)

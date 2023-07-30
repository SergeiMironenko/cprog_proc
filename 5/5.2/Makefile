Server=server
Client=client

make: clean server client

server:
	gcc -o $(Server) $(Server).c fun.c

client:
	gcc -o $(Client) $(Client).c fun.c

clean:
	rm -rf $(Server) $(Client)

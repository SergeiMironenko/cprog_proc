Server=server
Client=client
FUN=fun

make: clean server client

server: $(Server).c $(FUN).*
	gcc -o $(Server) $(Server).c $(FUN).c

client: $(Client).c $(FUN).*
	gcc -o $(Client) $(Client).c $(FUN).c

clean:
	rm -rf $(Server) $(Client)

Server=server
Client=client
FUN=fun

make: clean server client

server: $(Server).c $(FUN).*
	gcc -o $(Server) $(Server).c $(FUN).c -pthread

client: $(Client).c $(FUN).*
	gcc -o $(Client) $(Client).c $(FUN).c -pthread

clean:
	rm -rf $(Server) $(Client)

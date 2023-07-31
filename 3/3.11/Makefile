DEL=del
CHAT=chat
FILES=erproc.c
FLAGS=-lrt

make: clean chat del

chat: $(CHAT).c
	gcc -o $(CHAT) $(CHAT).c $(FILES) $(FLAGS)

del: $(DEL).c
	gcc -o $(DEL) $(DEL).c $(FILES) $(FLAGS)

clean:
	rm -rf $(DEL) $(CHAT)

DEL=del
P=p
FILES=erproc.c
FLAGS=-lrt

make: clean p del

p: $(P).c
	gcc -o $(P) $(P).c $(FILES) $(FLAGS)

del: $(DEL).c
	gcc -o $(DEL) $(DEL).c $(FILES) $(FLAGS)

clean:
	rm -rf $(DEL) $(P)

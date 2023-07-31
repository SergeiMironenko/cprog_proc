SND=snd
RCV=rcv
DEL=del
FILES=erproc.c
FLAGS=-lrt

make: clean snd rcv del

snd: $(SND).c
	gcc -o $(SND) $(SND).c $(FILES) $(FLAGS)

rcv: $(RCV).c
	gcc -o $(RCV) $(RCV).c $(FILES) $(FLAGS)

del: $(DEL).c
	gcc -o $(DEL) $(DEL).c $(FILES) $(FLAGS)

clean:
	rm -rf $(SND) $(RCV) $(DEL)

SND=snd
RCV=rcv
DEL=del

make: clean snd rcv del

snd:
	gcc -o $(SND) $(SND).c

rcv:
	gcc -o $(RCV) $(RCV).c

del:
	gcc -o $(DEL) $(DEL).c

clean:
	rm -rf $(SND) $(RCV) $(DEL)

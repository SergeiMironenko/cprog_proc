MAIN=p
FUN1=mysum

make: clean $(FUN1)
	gcc -o $(MAIN) $(MAIN).c

$(FUN1):
	gcc -o $(FUN1) $(FUN1).c

clean:
	rm -f $(MAIN) $(FUN1)

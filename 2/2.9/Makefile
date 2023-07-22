MAIN=p
TEST=t

make: clean main test

main:
	gcc -o $(MAIN) $(MAIN).c

test:
	gcc -o $(TEST) $(TEST).c

clean:
	rm -f $(MAIN) $(TEST)

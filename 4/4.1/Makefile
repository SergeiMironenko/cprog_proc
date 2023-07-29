GEN=gen
OUT=out
FIFO=fifofile

make: clean gen out

gen:
	gcc -o $(GEN) $(GEN).c

out:
	gcc -o $(OUT) $(OUT).c

clean:
	rm -rf $(GEN) $(OUT) $(FIFO)

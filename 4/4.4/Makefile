GEN=gen
OUT=out
FIFO=fifofile

make: clean gen out

gen:
	gcc -o $(GEN) $(GEN).c -pthread

out:
	gcc -o $(OUT) $(OUT).c -pthread

clean:
	rm -rf $(GEN) $(OUT) $(FIFO)

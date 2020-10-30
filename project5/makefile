p5: proj5.c
	mpicc -Ofast -o p5 proj5.c -lm 

test: proj5.c
	mpicc -g proj5.c -o p5 -lm
clean:
	rm -rf p5
	rm -f *.o

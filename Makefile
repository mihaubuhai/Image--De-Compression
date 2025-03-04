build: *.c essentials.h
	gcc -Wall $^ -o quadtree -g

clean:
	rm quadtree
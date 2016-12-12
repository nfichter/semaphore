all: compile1 compile2

compile1: sem.c 
	gcc -g sem.c -o sem

compile2: control.c
	gcc -g control.c -o control

clean:
	rm control
	rm sem
	rm story.txt

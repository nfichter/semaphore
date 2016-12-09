all: compile1 compile2

compile1: sem.c 
	gcc sem.c -o sem
	
compile2: control.c
	gcc control.c -o control

clean:
	rm *~

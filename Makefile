all: build install run

build:
	gcc -shared -fPIC main.c -o libvicerp.so -lhiredis -I/usr/include/hiredis -Wall

install:
	mv ./libvicerp.so ./plugins

run:
	./mpsvrrel64

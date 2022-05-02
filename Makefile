all: build install

build:
	gcc -shared -fPIC main.c -o libvicerp.so -lhiredis -I/usr/include/hiredis -Wall -I./

install:
	mv ./libvicerp.so ./plugins

run:
	./mpsvrrel64
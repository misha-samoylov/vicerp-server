all: build install run

build:
	gcc -shared -fPIC vicerp.c -o libvicerp.so -lhiredis -I/usr/include/hiredis

install:
	mv ./libvicerp.so ./plugins

run:
	./mpsvrrel64
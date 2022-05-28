all: build install

build:
	gcc -shared -fPIC ./src/main.c -o ./src/libvicerp.so -lhiredis -I/usr/include/hiredis -Wall -I./ `mysql_config --libs` `mysql_config --cflags`

install:
	mv ./src/libvicerp.so ./src/plugins

run:
	./mpsvrrel64

docker:
	docker-compose up -d
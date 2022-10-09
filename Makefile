all: build install docker-restart

build:
	gcc -shared -fPIC ./src/main.c -o ./src/libvicerp.so -lhiredis -I/usr/include/hiredis -Wall -I./ `mysql_config --libs` `mysql_config --cflags`

install:
	rm ./src/plugins/libvicerp.so
	mv ./src/libvicerp.so ./src/plugins

docker-restart:
	docker-compose down && docker-compose up -d
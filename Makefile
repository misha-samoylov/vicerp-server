all: build install

build:
	gcc -shared -fPIC main.c -o libvicerp.so -lhiredis -I/usr/include/hiredis -Wall

install:
	mv ./libvicerp.so ./plugins

run:
	./mpsvrrel64

docker-build:
	sudo docker build -t vicerp-server .

docker-run:
	sudo docker run --net my-net -d --name instance-vicerp-server --rm -p 8192:8192/udp vicerp-server

docker-stop:
	sudo docker stop instance-vicerp-server

docker-clean:
	sudo docker rmi vicerp-server

# install docker redis server and net
docker-install-redis:
	sudo docker pull redis
	sudo docker network create my-net

docker-run-redis:
	sudo docker run --net my-net -d --name redis-server --rm -p 6379:6379 redis

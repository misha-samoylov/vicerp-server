all: build install

build:
	gcc -shared -fPIC main.c -o libvicerp.so -lhiredis -I/usr/include/hiredis -Wall

install:
	mv ./libvicerp.so ./plugins

run:
	./mpsvrrel64

docker-build:
	docker build -t vicerp-server .

docker-run:
	docker run --net my-net \
	-d \
	--name instance-vicerp-server \
	--rm \
	-p 8192:8192/udp \
	-v ${CURDIR}/plugins:/home/appuser/plugins:ro \
	-v ${CURDIR}/server.cfg:/home/appuser/server.cfg:ro \
	vicerp-server

docker-stop:
	docker stop instance-vicerp-server

docker-clean:
	docker rmi vicerp-server

# install docker redis server and net
docker-install-redis:
	docker pull redis
	docker network create my-net

docker-run-redis:
	docker run --net my-net -d --name redis-server --rm -p 6379:6379 redis

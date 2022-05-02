#!/bin/sh

# Run redis server
docker run --net my-net \
    -d \
    --name redis-server \
    --rm \
    -p 6379:6379 \
    redis

# Run vicerp-server
docker run --net my-net \
	-d \
	--name instance-vicerp-server \
	--rm \
	-p 8192:8192/udp \
	-v ${CURDIR}/plugins:/home/appuser/plugins:ro \
	-v ${CURDIR}/server.cfg:/home/appuser/server.cfg:ro \
	vicerp-server
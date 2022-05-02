#!/bin/sh

# Create network
docker network create my-net

# Build redis server
docker pull redis
docker run --net my-net -d --name redis-server --rm -p 6379:6379 redis

# Build vicerp-server image
docker build -t vicerp-server .
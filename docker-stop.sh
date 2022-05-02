#!/bin/sh

# Stop docker container vicerp
docker stop instance-vicerp-server

# Stop docker container redis
docker stop redis-server
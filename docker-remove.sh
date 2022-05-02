#!/bin/sh

# Remove Docker image
docker rmi vicerp-server

# Remove network
docker network rm my-net
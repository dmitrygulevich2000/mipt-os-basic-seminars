#!/bin/bash

cat .env
source .env

docker run -d --rm \
    --name env \
    -v $HOST_WORKSPACE_DIR:/root/$(basename $HOST_WORKSPACE_DIR) \
    -p 2222:22 \
    dev-env

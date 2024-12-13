#!/bin/bash

echo "Preparing environment and running contaiter..."
cat .env || exit $?
source .env

docker run -d --rm \
    --name os-env \
    -v $HOST_WORKSPACE_DIR:/root/$(basename $HOST_WORKSPACE_DIR) \
    -p 2222:22 \
    os-dev-env:latest

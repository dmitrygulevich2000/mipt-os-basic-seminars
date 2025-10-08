#!/bin/bash

echo "Preparing environment and running contaiter..."
cat .env || exit $?
source .env

docker run -d \
    --name os-env \
    -v $HOST_WORKSPACE_DIR:/root/$(basename $HOST_WORKSPACE_DIR) \
    -v os-env-vscode-server:/root/.vscode-server \
    -p 2222:22 \
    os-dev-env:latest


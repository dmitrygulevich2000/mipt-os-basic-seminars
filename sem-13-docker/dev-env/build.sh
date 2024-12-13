#!/bin/bash

echo "Building image... This may take some time"
docker build -t os-dev-env:latest .

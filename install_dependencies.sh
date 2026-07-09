#!/bin/bash
set -e

sudo apt update

sudo apt install -y \
    build-essential \
    cmake \
    libcurl4-openssl-dev \
    libpqxx-dev \
    nlohmann-json3-dev

echo
echo "Dependencies installed successfully."

# 3AP_HTTPDataCollector

Simple HTTP telemetry collector written in C++.

The application periodically reads data from HTTP endpoints (e.g. PLCs, gateways or embedded devices), parses JSON responses and stores measurements in a PostgreSQL database.

## Features

- Load endpoint configuration from JSON
- HTTP communication using libcurl
- JSON parsing using nlohmann/json
- Store measurements in PostgreSQL
- Support multiple endpoints

## Dependencies

- CMake
- libcurl
- libpqxx
- PostgreSQL
- nlohmann/json

On Ubuntu:

```bash
./install_dependencies.sh
```

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Configuration

Edit:

```text
config/collector.json
```

to define your HTTP endpoints.

## Status

The project is functional and under active development.

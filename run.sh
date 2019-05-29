#!/usr/bin/env bash

NUM_CONNECTIONS=1000
NUM_SAMPLES=3
SERVER_PORT=5559

# Run benchmark against server by measuring the time it takes to create
# NUM_CONNECTIONS to it.
benchmark() {
    local start_server="$1"

    for i in $(seq 1 "$NUM_SAMPLES"); do
        printf "sample %d: " "$i"

        # Start server in background.
        eval "$start_server" &

        # Create NUM_CONNECTIONS to server.
        conns_created=0
        while [ "$conns_created" -lt "$NUM_CONNECTIONS" ]; do
            if ! nc -z 127.0.0.1 "$SERVER_PORT"; then
                continue
            fi

            conns_created=$((conns_created + 1))
        done

        wait
    done
}

# Print current time in millis.
now() {
    local nanos
    nanos=$(date +%s%N)

    echo $((nanos / 1000000))
}

# Starts a simple server.
start_simpleserver() {
    local name=$1
    local exe=$2

    local start_time
    start_time=$(now)

    eval "$exe"

    local end_time
    end_time=$(now)

    echo "$name accepted $NUM_CONNECTIONS connections in $((end_time - start_time))ms."
}

# Start the simple server written in C.
start_simpleserverc() {
    start_simpleserver "simpleserver-c" ./bin/simpleserver-c
}

# Start the simple server written in Python.
start_simpleserverpython() {
    start_simpleserver "simplerserver-python" ./simpleserver/python/server.py
}

benchmark "start_simpleserverc"
benchmark "start_simpleserverpython"

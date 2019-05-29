#!/usr/bin/python3.7

import socket
import sys

ADDRESS = "127.0.0.1"
PORT = 5559
MAX_CONNS = 1000

try:
    # Create, bind, listen in order to start accepting connections.
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Set SO_REUSEADDR to ensure we can bind if connections are still in
    # TIME_WAIT from last run.
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((ADDRESS, PORT))
    sock.listen()
    #print("listening on %d..." % PORT)

    # Accept connections until max conns is reached.
    accepted_conns = 0
    while accepted_conns < MAX_CONNS:
        # Accept and close immediately.
        conn, addr = sock.accept()
        accepted_conns += 1
        conn.close()

    sock.close()
except OSError as msg:
    print(msg)
    sys.exit(1)

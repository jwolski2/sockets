all: server

run:
	@./run.sh

server:
	gcc -o bin/simpleserver-c simpleserver/c/server.c

.PHONY: run server

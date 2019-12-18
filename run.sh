#!/bin/bash

if [ "$1" == "s" ]; then
	./build/example_server
else
	./build/example_client
fi

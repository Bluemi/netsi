#!/bin/bash

if [ "$1" == "s" ]; then
	./build/tests/example_server
else
	./build/tests/example_client
fi

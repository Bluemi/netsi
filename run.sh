#!/bin/bash

if [ "$1" == "s" ]; then
  ./build/debug/tests/bin/server
else
  ./build/debug/tests/bin/client
fi

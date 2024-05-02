#!/bin/bash

# Function to write every 10 secs to the log file

while true; do
  echo "Hello World" >./log.txt
  sleep 1
done
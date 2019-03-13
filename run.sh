#!/bin/bash
default='115200'
baudrate=${1:-$default}
sudo screen /dev/ttyUSB0 $baudrate

#!/bin/bash

killall qemu-system-i386 2> /dev/null

nohup "$(which qemu-system-i386)" \
  -machine q35 \
  -m 256 \
  -kernel kernel/kernel.bin \
  -s -S &

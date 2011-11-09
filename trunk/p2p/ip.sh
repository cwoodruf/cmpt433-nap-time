#!/bin/sh
ifconfig | grep addr: | grep -v addr:127 | sed -e 's/.*addr:\([0-9\.]*\).*/\1/'

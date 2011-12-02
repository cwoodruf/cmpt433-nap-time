#!/bin/sh
ip -f inet -o addr show eth0 | sed -e 's/.*inet *\([0-9\.]*\).*/\1/'

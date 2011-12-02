#!/bin/sh
# run this first on receiving host then run sendtest.sh on sending host (both can be the same host)
. /etc/profile
port=29528
echo rtp receive on $port
naprtprecv $port

#!/bin/sh
source /etc/p2p.conf
/usr/bin/wget -q -O - http://$WEBUSER:$PASSWORD@$WEBHOST/napmedia/index.php?action=node/ls

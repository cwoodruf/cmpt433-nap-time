#!/bin/sh
if [ ! -f src/boa ] 
then
	echo nap installation should be run from the boa build diretory
	return 0
fi

installdir=$1
if [ "$installdir" = "" ]
then
	echo usage: $0 {installdir}
	echo suggest using ../boa as the install dir
	return 0
fi

if [ -d $installdir ] 
then
	echo install dir $installdir found - copy examples/boa.conf manually
else
	mkdir "$installdir"
	cp examples/boa.conf "$installdir"
fi

cp src/boa "$installdir"
cat <<TXT
to run: 
 cd $installdir
 ./boa -c . \&
TXT

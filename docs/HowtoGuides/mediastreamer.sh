#!/bin/sh
# run in the directory where you wish to download files 
# assumes the $PR variable is set which points to our private directory
# this does not build linphone as ncurses does not seem to work on the board
wget http://mirror.csclub.uwaterloo.ca/nongnu/linphone/stable/sources/linphone-3.4.3.tar.gz
tar xzf linphone-3.4.3.tar.gz 
cd linphone-3.4.3/
cd oRTP
make clean
./configure --prefix=$PR/ortp --host=arm-linux CFLAGS="$ARMOPTS" CPPFLAGS="$ARMOPTS"
make 
make install
cd ../..
wget http://downloads.xiph.org/releases/ogg/libogg-1.3.0.tar.gz
tar xzf libogg-1.3.0.tar.gz 
cd libogg-1.3.0/
make clean
./configure --prefix=$PR/speex --host=arm-linux CFLAGS="$ARMOPTS" CPPFLAGS="$ARMOPTS"
make
make install
wget http://downloads.xiph.org/releases/speex/speex-1.2rc1.tar.gz
tar xzf speex-1.2rc1.tar.gz
cd speex-1.2rc1/
make clean
./configure --prefix=$PR/speex --host=arm-linux CFLAGS="-I$PR/speex/include $ARMOPTS" CPPFLAGS="-I$PR/speex/include $ARMOPTS" LDFLAGS="-L$PR/speex/lib"
make
make install
cd ../linphone-3.4.3/oRTP/
make clean
./configure --prefix=$PR/ortp --host=arm-linux CFLAGS="-I$PR/speex/include $ARMOPTS" CPPFLAGS="-I$PR/speex/include $ARMOPTS" LDFLAGS="-L$PR/speex/lib"
make
make install
cd ../mediastreamer2/
make clean
export SPEEX_CFLAGS="-I$PR/speex/include" SPEEX_LIBS="-L$PR/speex/lib -lspeex -lm -lspeexdsp"
./configure --prefix=$PR/ortp --host=arm-linux CFLAGS="-I$PR/speex/include -I$PR/ortp/include -I$PR/alsa/include $ARMOPTS" CPPFLAGS="-I$PR/speex/include -I$PR/ortp/include -I$PR/asla/include $ARMOPTS" LDFLAGS="-L$PR/speex/lib -L$PR/ortp/lib -L$PR/alsa/lib" --disable-video --disable-pulseaudio 
make
make install


install process:
  install zlib openssh boost libevent

  sudo apt-get install libssl-dev
  sudo apt-get install autoconf
  sudo apt-get install autotools-dev
  sudo apt-get install flex
  sudo apt-get install libtool
  sudo apt-get install byacc

  download from  http://thrift.apache.org/

  ./configure --with-boost=/home/guoliqiang/git-coding/coding/third_part/boost/  --with-libevent=/home/guoliqiang/git-coding/coding/third_part/libevent/ --with-zlib=/home/guoliqiang/git-coding/coding/third_part/zlib/ --with-openssl=/home/guoliqiang/git-coding/coding/third_part/openssh/ --prefix=/home/guoliqiang/git-coding/coding/third_part/thrift/ --with-erlang=false
  # you can see the configure out from configure.out file

  make

  make install

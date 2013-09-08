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

  g++ vertion:
   Using built-in specs.
   Target: x86_64-linux-gnu
   Configured with: ../src/configure -v --with-pkgversion='Ubuntu 4.4.3-4ubuntu5.1' --with-bugurl=file:///usr/share/doc/gcc-4.4/README.Bugs --enable-languages=c,c++,fortran,objc,obj-c++ --prefix=/usr --enable-shared --enable-multiarch --enable-linker-build-id --with-system-zlib --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.4 --program-suffix=-4.4 --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-plugin --enable-objc-gc --disable-werror --with-arch-32=i486 --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
   Thread model: posix
   gcc version 4.4.3 (Ubuntu 4.4.3-4ubuntu5.1)

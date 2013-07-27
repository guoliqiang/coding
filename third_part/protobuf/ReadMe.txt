./configure --prefix=. --with-zlib
make
make check
make install

find . -name "*.h" | xargs sed -i 's/<google/<third_part\/protobuf\/include\/google/'

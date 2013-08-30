# (before make shared library)./configure --prefix=. --with-zlib

./configure  --disable-shared  --prefix=/home/guoliqiang/git-coding/coding/third_part/protobuf/  LDFLAGS=-static --enable-static
modify 'configure' file


    5102                                         if test "x${ac_cv_env_CXXFLAGS_set}" = "x"; then :
    5103 
    5104             CXXFLAGS="${CXXFLAGS} -m64 -fPIC"
    5105             ac_cv_env_CXXFLAGS_set=set
    5106             ac_cv_env_CXXFLAGS_value='-m64'
    5107 
    5108 fi
    5109 
    5110           if test "x${ac_cv_env_CFLAGS_set}" = "x"; then :
    5111 
    5112             CFLAGS="${CFLAGS} -m64 -fPIC"
    5113             ac_cv_env_CFLAGS_set=set                                                                                                                    
    5114             ac_cv_env_CFLAGS_value='-m64'

add "-fPIC"


make
make check
make install

find . -name "*.h" | xargs sed -i 's/<google/<third_part\/protobuf\/include\/google/'


Reference:
http://hi.baidu.com/einstein17/item/4bf81447421803e11281dac6
http://en.it-usenet.org/thread/15022/8058/
http://markmail.org/message/lfxtb4inhjpbsh4x#query:+page:1+mid:qtld2lpohewsavyp+state:results
https://code.google.com/p/protobuf/issues/detail?id=370&q=static%20library&colspec=ID%20Type%20Status%20Priority%20FixedIn%20Owner%20Summary

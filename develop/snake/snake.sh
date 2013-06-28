#!/bin/bash
# use example:
#  bash devel/scons/build.sh [t=target] [c=opt]

original_dir=`pwd -P`
cd -P `dirname $0`/../../
base_dir=`pwd -P`

if [ -z $base_dir ]; then
  echo "Should run $0 under the ROOT of the code base!"
  exit
fi

cd -P .  # go to real path if this is a symbol link.
cd $base_dir

scons \
--site-dir=$base_dir/develop/snake/site_scons \
--f=$base_dir/develop/snake/SConstruct \
cur_dir=$original_dir \
base_dir=$base_dir \
scons_dir=$base_dir/third_part/scons/ \
python_dir=$base_dir/third_part/python/ \
thrift_bin=$base_dir/third_part/thrift/bin/thrift \
-Q \
--jobs=4 \
$@

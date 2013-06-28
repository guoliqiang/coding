#!/bin/sh

../mkdarts linux.words linux.words.dar
echo "Checking ... "
../darts linux.words.dar < linux.words | grep "not found"

exit 0



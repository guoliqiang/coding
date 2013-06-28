FILE(REMOVE_RECURSE
  "CMakeFiles/mylib.dir/lib.cc.o"
  "libmylib.pdb"
  "libmylib.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/mylib.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

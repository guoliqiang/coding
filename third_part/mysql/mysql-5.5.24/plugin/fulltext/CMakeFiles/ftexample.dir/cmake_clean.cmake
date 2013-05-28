FILE(REMOVE_RECURSE
  "CMakeFiles/ftexample.dir/plugin_example.c.o"
  "mypluglib.pdb"
  "mypluglib.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/ftexample.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

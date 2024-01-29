file(REMOVE_RECURSE
  "libUtilsLib.a"
  "libUtilsLib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/UtilsLib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

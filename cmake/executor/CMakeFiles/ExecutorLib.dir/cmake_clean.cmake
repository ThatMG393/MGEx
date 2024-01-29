file(REMOVE_RECURSE
  "libExecutorLib.a"
  "libExecutorLib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/ExecutorLib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

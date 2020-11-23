file(REMOVE_RECURSE
  "libsettings.a"
  "libsettings.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/settings.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Bible_autogen"
  "CMakeFiles/Bible_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Bible_autogen.dir/ParseCache.txt"
  )
endif()

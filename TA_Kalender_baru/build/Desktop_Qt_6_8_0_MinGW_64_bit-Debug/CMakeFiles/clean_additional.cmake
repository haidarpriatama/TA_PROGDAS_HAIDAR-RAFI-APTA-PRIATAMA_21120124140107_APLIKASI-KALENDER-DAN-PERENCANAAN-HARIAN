# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TA_Kalender_baru_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TA_Kalender_baru_autogen.dir\\ParseCache.txt"
  "TA_Kalender_baru_autogen"
  )
endif()

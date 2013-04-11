SET(SRCs
  toolDisplay)

SET(CMAKE_CXX_FLAGS "-std=c++0x") 

FOREACH(FILE ${SRCs})
  add_executable(${FILE} ${FILE})
  target_link_libraries( ${FILE} ${DGTAL_LIBRARIES} )
ENDFOREACH(FILE)

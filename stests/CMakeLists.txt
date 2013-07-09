SET(SRCs
  testPointVector2D
  testRay
  testConvergents
  testConvexHull
  testNegativeAlphaShapeStraightLine
  testNegativeAlphaShape
  testPositiveAlphaShape  
)

FOREACH(FILE ${SRCs})
  add_executable(${FILE} ${FILE})
  target_link_libraries( ${FILE} ${DGTAL_LIBRARIES} )
  add_test(${FILE} ${FILE})
ENDFOREACH(FILE)

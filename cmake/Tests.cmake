add_custom_target(tests)

macro(mimosa_test MODULE SOURCES)
  add_executable(tst-${MODULE} EXCLUDE_FROM_ALL ${SOURCES})
  target_link_libraries(tst-${MODULE} ${MODULE} gtest_main gtest pthread)
  add_custom_target(run-tst-${MODULE} ./tst-${MODULE} DEPENDS tst-${MODULE})
  add_dependencies(tests run-tst-${MODULE})
endmacro(mimosa_test)

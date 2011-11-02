add_custom_target(tests)

macro(mimosa_test MODULE)
  add_executable(tst-${MODULE} EXCLUDE_FROM_ALL ${ARGN})
  target_link_libraries(tst-${MODULE} gtest_main gtest pthread)
  add_custom_target(run-tst-${MODULE} ./tst-${MODULE} DEPENDS tst-${MODULE})
  add_dependencies(tests run-tst-${MODULE})
endmacro(mimosa_test)

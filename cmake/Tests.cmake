add_custom_target(check)

macro(mimosa_test MODULE)
  add_executable(tst-${MODULE} ${ARGN})
  target_link_libraries(tst-${MODULE} gtest_main gtest mimosa)
  add_custom_target(run-tst-${MODULE} ./tst-${MODULE} DEPENDS tst-${MODULE})
  add_dependencies(check run-tst-${MODULE})
endmacro(mimosa_test)

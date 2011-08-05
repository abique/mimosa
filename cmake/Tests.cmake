macro(mimosa_test MODULE SOURCES)
  add_executable(tst-${MODULE} EXCLUDE_FROM_ALL ${SOURCES})
  target_link_libraries(tst-${MODULE} ${MODULE} gtest_main gtest pthread)
endmacro(mimosa_test)

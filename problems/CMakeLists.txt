add_compile_options(-std=c++14 -Wfatal-errors)

macro(add_problem name)
  set(test_target "TEST_${name}")
  add_executable(${test_target} Contest.cc)

  add_library(${name} "${name}/main.cc")

  target_link_libraries(${test_target} ${name})
  target_link_libraries(${test_target} gtest_main)

  add_test(NAME ${name} COMMAND ${test_target}
           WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${name}/tests")
endmacro(add_problem)

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro(subdirlist)

set(dirs "")
subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})
foreach(subdir ${dirs})
  if(BUILD_ALL OR ${subdir})
    add_problem(${subdir})
  endif()
endforeach()

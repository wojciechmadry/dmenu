set(CMAKE_BUILD_TYPE Debug)
SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(
  drw_tests
  drw_tests.cpp
  mocks/X11/Xlib.cpp
  ../drw.cpp
  ../util.cpp
)

target_link_libraries(
  drw_tests
  GTest::gtest_main
  GTest::gmock_main
  gcov
)
target_include_directories(
  drw_tests
  PUBLIC
  mocks
)

include(GoogleTest)
gtest_discover_tests(drw_tests)


add_custom_target(LCOV
  lcov
  --ignore-errors inconsistent,mismatch,empty
  --directory ${PROJECT_SOURCE_DIR}
  --no-external
  --capture
  --output-file code_coverage.info
  -rc branch_coverage=1
  --exclude="${PROJECT_SOURCE_DIR}/googletest"
  --exclude="${PROJECT_SOURCE_DIR}/tests"
)
add_custom_target(coverage
  genhtml code_coverage.info
  --branch-coverage
  --output-directory ${PROJECT_BINARY_DIR}/code_coverage_report/
  DEPENDS LCOV
)

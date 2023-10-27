set(TEST_NAME "${PROJECT_NAME}_test")

project(${TEST_NAME})

include_directories(.
  ../src
)

add_executable(${PROJECT_NAME}
  test.cpp
  main.cpp
)

find_package(GTest)
find_package(Threads)
find_package(Qt6)

target_link_libraries(${PROJECT_NAME}
  GTest::GTest
  Threads::Threads
  Qt6::Qt6
	${CMAKE_DL_LIBS}
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

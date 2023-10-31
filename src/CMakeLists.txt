add_executable(${PROJECT_NAME}
  debug.h
  Board.h
  Board.cpp
  BoardWindow.h
  BoardWindow.cpp
  Bot.h
  Bot.cpp
  MainMenuWindow.h
  MainMenuWindow.cpp
  main.cpp
)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		/W4 # Baseline reasonable warnings
		/permissive- # standards conformance mode for MSVC compiler.
    /utf-8
	)
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE
		-Wall # lots of warnings
		-Wextra # reasonable and standard
		-Wpedantic # warn if non-standard C++ is used
		-Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This help catch hard to track down memory errors
		-Woverloaded-virtual # warn if you overload (not override) a virtual function
		-Wnull-dereference # warn if a null dereference is detected
		-Wextra-semi # Warn about redundant semicolons after in-class function definitions
	)
endif()

find_package(Threads)
find_package(Qt6)

target_link_libraries(${PROJECT_NAME}
  Threads::Threads
  Qt6::Qt6
	${CMAKE_DL_LIBS}
)

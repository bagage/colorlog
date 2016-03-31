cmake_minimum_required (VERSION 2.6)

# MACOSX_RPATH should be enabled
cmake_policy(SET CMP0042 NEW)

option(BUILD_TESTS "Build all of unit tests." ON)
project (color)

include(GNUInstallDirs)

# temporary disable packages for now; linking statically with boost is
# not working well - to be investigated...
# add_subdirectory(CMake)
# set(Boost_USE_STATIC_LIBS   ON)
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

FIND_PACKAGE( Boost 1.38 COMPONENTS regex REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


# Set default build type
message(STATUS "${PROJECT_NAME}: Build type - ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -fstack-protector-all -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O3")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_BINARY_DIR}")
set (BASE_SRC_DIR "${CMAKE_SOURCE_DIR}/src")

file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/data/color.conf
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/color.conf
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}
    COMPONENT configuration)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/color.1
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
    COMPONENT manual)

if (EXISTS ${CMAKE_INSTALL_FULL_DATAROOTDIR}/zsh/functions/Completion)
    message ("found zsh completion folder: gonna install color.zsh in it, great!")
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/color.zsh
	    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/zsh/functions/Completion/
	    COMPONENT completion
        RENAME _color)
endif()

if (BUILD_TESTS)
    # setup testing
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(src)

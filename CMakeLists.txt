# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Fetch SFML 3.0 dependency
include(FetchContent)
FetchContent_Declare(SFML
	GIT_REPOSITORY https://github.com/SFML/SFML
	GIT_TAG a71d6bc078d653c719e7ad96b3812028a81749b2)
FetchContent_MakeAvailable(SFML)


project ("LuaTest")

# Include sub-projects.
add_subdirectory("LuaTest")

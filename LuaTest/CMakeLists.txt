add_library(Lua SHARED IMPORTED)
set_target_properties(Lua PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Lua/lua54.dll"
  IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Lua/lua54.lib"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Lua/include"
)

add_executable(LuaTest "LuaTest.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LuaTest PROPERTY CXX_STANDARD 20)
endif()

target_compile_definitions(LuaTest PRIVATE -DLUA_BUILD_AS_DLL)
target_link_libraries(LuaTest PRIVATE Lua)
target_link_libraries(LuaTest PRIVATE SFML::Graphics SFML::Audio SFML::System)
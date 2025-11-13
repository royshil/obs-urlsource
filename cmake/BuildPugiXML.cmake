include(ExternalProject)

if(APPLE)
  set(PUGIXML_CMAKE_PLATFORM_OPTIONS -DCMAKE_OSX_ARCHITECTURES=x86_64$<SEMICOLON>arm64)
else()
  if(UNIX)
    set(PUGIXML_CMAKE_PLATFORM_OPTIONS -DCMAKE_POSITION_INDEPENDENT_CODE=ON)
  else()
    set(PUGIXML_CMAKE_PLATFORM_OPTIONS "")
  endif()
endif()

set(pugixml_lib_filename ${CMAKE_STATIC_LIBRARY_PREFIX}pugixml${CMAKE_STATIC_LIBRARY_SUFFIX})

ExternalProject_Add(
  pugixml_build
  URL https://github.com/zeux/pugixml/releases/download/v1.15/pugixml-1.15.tar.gz
  URL_HASH SHA256=655ade57fa703fb421c2eb9a0113b5064bddb145d415dd1f88c79353d90d511a
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  INSTALL_BYPRODUCTS <INSTALL_DIR>/include
  BUILD_BYPRODUCTS <INSTALL_DIR>/lib/${pugixml_lib_filename}
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_SHARED_LIBS=OFF -DPUGIXML_BUILD_TESTS=OFF
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ${PUGIXML_CMAKE_PLATFORM_OPTIONS} -DCMAKE_INSTALL_LIBDIR=lib
)

ExternalProject_Get_Property(pugixml_build INSTALL_DIR)

set(pugixml_lib_location ${INSTALL_DIR}/lib/${pugixml_lib_filename})

add_library(pugixml_internal STATIC IMPORTED)
add_dependencies(pugixml_internal pugixml_build)
set_target_properties(pugixml_internal PROPERTIES IMPORTED_LOCATION ${pugixml_lib_location})
target_include_directories(pugixml_internal INTERFACE ${INSTALL_DIR}/include)

add_library(libpugixml_internal INTERFACE)
add_dependencies(libpugixml_internal pugixml_build pugixml_internal)
target_link_libraries(libpugixml_internal INTERFACE pugixml_internal)

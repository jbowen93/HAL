find_package( freenect2 QUIET )

set( BUILD_Freenect2 "${freenect2_FOUND}" CACHE BOOL
  "Build Freenect2 driver for Kinect2" )

if( BUILD_Freenect2 )
    if( freenect2_FOUND )
        add_to_hal_include_dirs( ${freenect2_INCLUDE_DIRS} )
        add_to_hal_libraries( ${freenect2_LIBRARY} )
        add_to_hal_sources( Freenect2Driver.h Freenect2Driver.cpp
          Freenect2Factory.cpp )
	hal_set_compile_flags( ${CMAKE_CURRENT_SOURCE_DIR}/Freenect2Driver.cpp ${freenect2_DEFINITIONS} )
	hal_set_compile_flags( ${CMAKE_CURRENT_SOURCE_DIR}/Freenect2Factory.cpp ${freenect2_DEFINITIONS} )
    else()
	message("libfreenect2 not found. Freenect2 driver cannot be built")
    endif()
endif()

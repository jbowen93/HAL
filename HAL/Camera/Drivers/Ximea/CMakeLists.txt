set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH} )


find_package( XIMEA QUIET )

if( XIMEA_FOUND )

    set( BUILD_Ximea true CACHE BOOL force )

    if( BUILD_Ximea )
        add_to_hal_include_dirs( ${XIMEA_INCLUDE_DIR} )
        add_to_hal_libraries( ${XIMEA_LIBRARIES} )
        add_to_hal_sources(
            XimeaDriver.h XimeaDriver.cpp XimeaFactory.cpp
        )
    endif()
endif()

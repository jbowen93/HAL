if(BUILD_NodeEncoder)
  find_package(Node QUIET)
  find_package(GLog QUIET)

  if(Node_FOUND AND GLOG_FOUND)
    add_to_hal_include_dirs(${Node_INCLUDE_DIRS} ${GLog_INCLUDE_DIRS})
    add_to_hal_libraries(${Node_LIBRARIES} ${GLog_LIBRARIES})
    add_to_hal_sources(NodeEncoderDriver.h NodeEncoderDriver.cpp NodeEncoderFactory.cpp)
  else(Node_FOUND)
    message("Node not found. NodeEncoder driver cannot be built")
  endif()
endif()

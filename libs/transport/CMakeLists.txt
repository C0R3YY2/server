ADD_LIBRARY(transport STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(transport PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_transport.a")


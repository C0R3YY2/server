ADD_LIBRARY(rc_calib STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(rc_calib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_rc_calib.a")


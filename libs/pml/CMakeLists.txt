ADD_LIBRARY(pml STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(pml PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_pml.a")

ADD_LIBRARY(pml_di03 STATIC IMPORTED GLOBAL)
SET_TARGET_PROPERTIES(pml_di03 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib_pml_di03.a")


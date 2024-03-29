# Author: Nicolas Tisserand

MACRO   (CHANGE_FLAG VAR_NAME FROM_FLAG TO_FLAG)
  STRING(REPLACE ${FROM_FLAG} ${TO_FLAG} TMP ${${VAR_NAME}})
  SET(${VAR_NAME} ${TMP} CACHE STRING "" FORCE)
ENDMACRO(CHANGE_FLAG)

MACRO   (ADD_FLAG VAR_NAME FLAG)
  IF   (NOT ${VAR_NAME} MATCHES ${FLAG})
    SET(${VAR_NAME} "${${VAR_NAME}} ${FLAG}" CACHE STRING "" FORCE)
  ENDIF(NOT ${VAR_NAME} MATCHES ${FLAG})
ENDMACRO(ADD_FLAG)

MACRO   (USE_STATIC_MSVC_RUNTIME_LIBRARIES)
  CHANGE_FLAG(CMAKE_C_FLAGS_DEBUG "/MDd" "/MTd")
  CHANGE_FLAG(CMAKE_C_FLAGS_RELEASE "/MD" "/MT")
  CHANGE_FLAG(CMAKE_C_FLAGS_MINSIZEREL "/MD" "/MT")
  CHANGE_FLAG(CMAKE_C_FLAGS_RELWITHDEBINFO "/MD" "/MT")
  
  CHANGE_FLAG(CMAKE_CXX_FLAGS_DEBUG "/MDd" "/MTd")
  CHANGE_FLAG(CMAKE_CXX_FLAGS_RELEASE "/MD" "/MT")
  CHANGE_FLAG(CMAKE_CXX_FLAGS_MINSIZEREL "/MD" "/MT")
  CHANGE_FLAG(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD" "/MT")

  # Prevent linker warning when using release builds in debug mode.
  ADD_FLAG(   CMAKE_EXE_LINKER_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT")
  ADD_FLAG(CMAKE_MODULE_LINKER_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT")
  ADD_FLAG(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT")  
ENDMACRO(USE_STATIC_MSVC_RUNTIME_LIBRARIES)

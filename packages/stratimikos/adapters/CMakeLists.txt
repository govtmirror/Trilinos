INCLUDE(AssertDefined)

ASSERT_DEFINED(${PACKAGE_NAME}_ENABLE_Ifpack)
IF (${PACKAGE_NAME}_ENABLE_Ifpack)
  ADD_SUBDIRECTORY(ifpack)
ENDIF()

ASSERT_DEFINED(${PACKAGE_NAME}_ENABLE_ML)
IF (${PACKAGE_NAME}_ENABLE_ML)
  ADD_SUBDIRECTORY(ml)
ENDIF()

ASSERT_DEFINED(${PACKAGE_NAME}_ENABLE_Amesos)
IF (${PACKAGE_NAME}_ENABLE_Amesos)
  ADD_SUBDIRECTORY(amesos)
ENDIF()

ASSERT_DEFINED(${PACKAGE_NAME}_ENABLE_AztecOO)
ASSERT_DEFINED(${PACKAGE_NAME}_ENABLE_EpetraExt)
IF (${PACKAGE_NAME}_ENABLE_AztecOO AND ${PACKAGE_NAME}_ENABLE_EpetraExt)
  ADD_SUBDIRECTORY(aztecoo)
ENDIF()

ASSERT_DEFINED(${PACKAGE_NAME}_ENABLE_Belos)
IF (${PACKAGE_NAME}_ENABLE_Belos)
  ADD_SUBDIRECTORY(belos)
ENDIF()

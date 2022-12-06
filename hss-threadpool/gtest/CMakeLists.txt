
SET(PROJ_NAME_TP_V10_GTEST hss_tp_v10_gtest)


# Create Test Executable
#---------------------------------------------------#
ADD_EXECUTABLE(${PROJ_NAME_TP_V10_GTEST}
    src/main.cpp
    src/GT_hss_tp_v10.cpp
  )

TARGET_LINK_LIBRARIES(${PROJ_NAME_TP_V10_GTEST} PUBLIC
  gtest
  gtest_main
  hss_thread_pool_v10
  hss_ts_queue
  MPMCQueue
  )

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME_TP_V10_GTEST} PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/bin
  ${CMAKE_CURRENT_SOURCE_DIR}/../tp/include/tpv10
  ${GOOGLETEST_PATH}/googletest/include
  ${CMAKE_INSTALL_FULL_INCLUDEDIR}/include
  ${HSSTSQUEUE_PATH}/include
  ${HSS_MPMCQUEUE_PATH}/include/rigtorp
  )

SET_TARGET_PROPERTIES(hss_thread_pool_v10 PROPERTIES LINKER_LANGUAGE CXX)

# Create test for ctest.
#---------------------------------------------------#
add_test(NAME ${PROJ_NAME_TP_V10_GTEST} COMMAND ${PROJ_NAME_TP_V10_GTEST})

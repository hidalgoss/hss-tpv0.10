
#SET(PROJ_NAME_HSSTP hss_main_tp)


#---------------------------------------------------#
#---------------------------------------------------#
#---------------------------------------------------#
# Libraries.
#---------------------------------------------------#
#---------------------------------------------------#
#---------------------------------------------------#



# Create Library
#---------------------------------------------------#
ADD_LIBRARY(hss_thread_pool_v10 SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_sensor_v10.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_publisher_v10.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_ipublisher_v10.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_subscriber_v10.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_isubscriber_v10.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_observer_middleware_v10.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_taskmanager_publisher_v10.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10/hss_task_v10.hpp
  )

TARGET_LINK_LIBRARIES(hss_thread_pool_v10 PUBLIC
  pthread
  hss_ts_queue
  hss_bmrk_utils
  )

TARGET_INCLUDE_DIRECTORIES(hss_thread_pool_v10 PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tpv10
  ${HSSTSQUEUE_PATH}/include
  ${HSSBMRKUTILS_PATH}/include
  ${GOOGLETEST_PATH}/googletest/include
  ${CMAKE_INSTALL_FULL_INCLUDEDIR}/include
  )

SET_TARGET_PROPERTIES(hss_thread_pool_v10 PROPERTIES LINKER_LANGUAGE CXX)






## Create Library
##---------------------------------------------------#
#ADD_LIBRARY(hss_thread_utils SHARED
#  ${CMAKE_CURRENT_SOURCE_DIR}/include/hss_thread_utils.hpp
#  )

#TARGET_LINK_LIBRARIES(hss_thread_utils PUBLIC
#  pthread
#  #Threads::Threads
#  )

#TARGET_INCLUDE_DIRECTORIES(hss_thread_utils PUBLIC
#  ${CMAKE_CURRENT_SOURCE_DIR}/include
#  )

#SET_TARGET_PROPERTIES(hss_thread_utils PROPERTIES LINKER_LANGUAGE CXX)







#---------------------------------------------------#
#---------------------------------------------------#
#---------------------------------------------------#
# Executables.
#---------------------------------------------------#
#---------------------------------------------------#
#---------------------------------------------------#


## Create Executable
##---------------------------------------------------#
#ADD_EXECUTABLE(${PROJ_NAME_HSSTP}
#  src/main_tp.cpp
#  )

#TARGET_LINK_LIBRARIES(${PROJ_NAME_HSSTP} PUBLIC
#  #hss_thread_utils
#  hss_thread_pool_v1
#  hss_benchmark_utils
#  hss_chrono
#  pthread
#  hss_lock_free_queue
#  #Threads::Threads
#  )

#TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME_HSSTP} PUBLIC
#  ${CMAKE_CURRENT_SOURCE_DIR}/include
#  ${CMAKE_CURRENT_SOURCE_DIR}/../bmrk/include
#  ${CMAKE_CURRENT_SOURCE_DIR}/../chrono/include
#  ${CMAKE_CURRENT_SOURCE_DIR}/../learning/include
#  )

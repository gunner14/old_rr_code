
INCLUDE_DIRECTORIES( 
	${XNS_BUILD_DIR}/thrift-gen
	/data/home/yuwei.mu/xce/include/zookeeper-client
  )

MACRO (GENERATE_THRIFT_RULES directory)
  FILE(GLOB THRIFT_SOURCES
    RELATIVE "${XNS_ROOT_DIR}/${directory}" 
    "${XNS_ROOT_DIR}/${directory}/*.thrift")
  MESSAGE("THRIFT_SOURCES : ${THRIFT_SOURCES}")
  SET(output_dir "${XNS_BUILD_DIR}/thrift-gen")

  #INCLUDE_DIRECTORIES("/opt/thrift-0.8.0/include/thrift" "${output_dir}/gen-cpp/")
  INCLUDE_DIRECTORIES("/data/home/yuwei.mu/xce/include/thrift" "${output_dir}/gen-cpp/")

  FOREACH(thrift_source ${THRIFT_SOURCES})
    STRING(REGEX REPLACE "\\.thrift" "_constants.cpp" constants_cpp "${thrift_source}" )
    MESSAGE(${constants_cpp})
    SET(out_file "${output_dir}/gen-cpp/${constants_cpp}")

    STRING(REGEX REPLACE "\\.thrift" "_types.cpp" types_cpp "${thrift_source}" )
    MESSAGE(${types_cpp})
    SET(out_file "${out_file}" "${output_dir}/gen-cpp/${types_cpp}")

    STRING(REGEX REPLACE "\\.thrift" ".cpp" stub_cpp "${thrift_source}" )
    MESSAGE(${stub_cpp})
    SET(out_file "${out_file}" "${output_dir}/gen-cpp/${stub_cpp}")

    STRING(REGEX REPLACE "\\.thrift" ".cpp" skeleton_cpp "${thrift_source}" )
    MESSAGE(${skeleton_cpp})
    SET(out_file "${out_file}" "${output_dir}/gen-cpp/${skeleton_cpp}")

    MESSAGE(${out_file})

    add_custom_command(OUTPUT ${out_file}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir}
      # COMMAND ${CMAKE_COMMAND} -E make_directory "${output_dir}" # 不能带引号!
      COMMAND thrift -o "${output_dir}" -gen cpp "${XNS_ROOT_DIR}/${directory}/${thrift_source}"
		  DEPENDS ${XNS_ROOT_DIR}/${directory}/${thrift_source}
      ) 

    STRING(REGEX REPLACE "\\.thrift" "" libname "${thrift_source}" )
    MESSAGE("add library OceThrift_${libname} ...")
    add_library("OceThrift_${libname}" STATIC ${out_file}) 
    APPEND( ADDITIONAL_MAKE_CLEAN_FILES ${out_file} )

    TARGET_LINK_LIBRARIES("OceThrift_${libname}" thrift)
  ENDFOREACH(thrift_source ${THRIFT_SOURCES})
ENDMACRO (GENERATE_THRIFT_RULES)

MACRO ( ADD_XOA_COMPONENT directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${libraryname}_SRC_CPP_SOURCE )
	MESSAGE(STATUS "xoa DEBUG: ${BUILD_${libraryname}_SRC_CPP_SOURCE}")
	ADD_LIBRARY( ${libraryname} STATIC EXCLUDE_FROM_ALL ${BUILD_${libraryname}_SRC_CPP_SOURCE})
	INCLUDE_DIRECTORIES(${XNS_ROOT_DIR}/${directoryname})
	TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN} )
ENDMACRO ( ADD_XOA_COMPONENT )

MACRO ( ADD_XOA_TEST_CASE directoryname test_case_name)
	MESSAGE(STATUS "DEBUG: ${ARGN}")
	ADD_EXECUTABLE( ${test_case_name} EXCLUDE_FROM_ALL ${XNS_ROOT_DIR}/${directoryname}/${test_case_name}.cc)
	TARGET_LINK_LIBRARIES( ${test_case_name} ${ARGN} )
ENDMACRO ( ADD_XOA_TEST_CASE )

GENERATE_THRIFT_RULES("xoa2/thrift-idl") 

ADD_XOA_COMPONENT("xoa2/client" "Xoa2Client" zookeeper_mt)
#ADD_XOA_TEST_CASE("xoa2/test" "xoa_registry_client_test" Xoa2Client)
ADD_XOA_TEST_CASE("xoa2/test" "client_pool_test" Xoa2Client OceThrift_SearchEngine)
ADD_XOA_TEST_CASE("xoa2/test" "twitter_test" Xoa2Client OceThrift_TwitterService)

#ADD_SERVICE_EXEC("user/UserScanner/test" "Zombie" ZombieUserScanner UserFilter ReplicaCluster OceCxxAdapterUserStateAdapter OceSliceReplicaCluster OceSliceSubjectObserver OceSliceUserBase DbCxxPool IceExt OceSliceXceStorm  UtilCxx UtilSlice)
ADD_SERVICE("xoa2/HelloWorld" "HelloService" ReplicaCluster IceExt Xoa2Client OceSliceHelloXoa OceSliceReplicaCluster OceSliceSubjectObserver OceSliceTalkCommon OceSliceXceStorm DbCxxPool UtilCxx UtilSlice fcgi ctemplate)
ADD_SERVICE_EXEC("xoa2/HelloClient" "HelloClient" OceSliceHelloXoa pthread Xoa2Client Ice IceUtil)
ADD_SERVICE_EXEC("xoa2/HelloPooledClient" "HelloPooledClient" OceSliceHelloXoa Xoa2Client Ice IceUtil)


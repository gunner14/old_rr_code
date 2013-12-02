## Copyright 2012-11-28 zhe.long@renren-inc.com


INCLUDE_DIRECTORIES(${SERVERLIB_ROOT}/../include/zookeeper-client)

MACRO (GENERATE_THRIFT_RULES directory)
  FILE(GLOB THRIFT_SOURCES
    RELATIVE "${SERVERLIB_ROOT}/${directory}" 
    "${SERVERLIB_ROOT}/${directory}/*.thrift")
  MESSAGE("THRIFT_SOURCES : ${THRIFT_SOURCES}")
  SET(output_dir "${SERVERLIB_ROOT}/thrift-gen")

  
  #INCLUDE_DIRECTORIES("/opt/thrift-0.8.0/include/thrift" "${output_dir}/gen-cpp/")
  INCLUDE_DIRECTORIES("${SERVERLIB_ROOT}/../include/thrift" "${output_dir}/gen-cpp/")
  FOREACH(thrift_source ${THRIFT_SOURCES})
    STRING(REGEX REPLACE "\\.thrift" "_constants.cpp" constants_cpp "${thrift_source}" )
    MESSAGE(${constants_cpp})
    SET(out_file "${output_dir}/gen-cpp/${constants_cpp}")

    STRING(REGEX REPLACE "\\.thrift" "_types.cpp" types_cpp "${thrift_source}" )
    MESSAGE(${types_cpp})
    SET(out_file "${out_file}" "${output_dir}/gen-cpp/${types_cpp}")

    #命名规则：
    #文件名为xxxx.thrift   里面的服务名为 xxxxService    
    STRING(SUBSTRING "${thrift_source}" "0" "1" first_letter)
    STRING(TOUPPER  "${first_letter}" first_big_letter)
    STRING(LENGTH "${thrift_source}" letter_length)
    MATH(EXPR letter_length "${letter_length}-1")
    STRING(SUBSTRING "${thrift_source}" "1" "${letter_length}" letter)
    STRING(REGEX REPLACE "\\.thrift" "Service.cpp" main_cpp "${first_big_letter}${letter}" )
    MESSAGE(${main_cpp} "================")
    SET(out_file "${out_file}" "${output_dir}/gen-cpp/${main_cpp}")


    MESSAGE(${out_file})
    ADD_CUSTOM_COMMAND(OUTPUT ${out_file}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir}
      # COMMAND ${CMAKE_COMMAND} -E make_directory "${output_dir}" # 不能带引号!
      COMMAND thrift -o "${output_dir}" -gen cpp "${SERVERLIB_ROOT}/${directory}/${thrift_source}"
      DEPENDS ${SERVERLIB_ROOT}/${directory}/${thrift_source}
      ) 
    
    STRING(REGEX REPLACE "\\.thrift" "" libname "${thrift_source}" )
    MESSAGE("add library ${libname} ...")
    add_library("OceThrift_${libname}" STATIC ${out_file})
    INSTALL(TARGETS "OceThrift_${libname}" ARCHIVE DESTINATION ${SERVERLIB_ROOT}/lib)
    
    TARGET_LINK_LIBRARIES("OceThrift_${libname}" thrift)
  ENDFOREACH(thrift_source ${THRIFT_SOURCES})
ENDMACRO (GENERATE_THRIFT_RULES)

GENERATE_THRIFT_RULES("thrift-idl") 



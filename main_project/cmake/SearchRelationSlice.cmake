
SET( SEARCH_RELATION_SLICE_LIB_NAME "SearchRelationSlice")
SET( SEARCHRELATIONSLICE_SRC_DIR "${XNS_ROOT_DIR}/search/SearchRelation/SearchRelationSlice/slice")
SET( SEARCHRELATIONSLICE_CPP_DIR "${XNS_BUILD_DIR}/tmp/${SEARCH_RELATION_SLICE_LIB_NAME}/cpp")
FILE( MAKE_DIRECTORY ${SEARCHRELATIONSLICE_CPP_DIR} )
#
# Slice
#
FILE( GLOB slice_source RELATIVE ${SEARCHRELATIONSLICE_SRC_DIR} "${SEARCHRELATIONSLICE_SRC_DIR}/*.ice" )
#SET ( XNS_SEARCHRELATIONSLICE_SOURCE_FILES ${slice_source} CACHE INTERNAL "DistCacheSlice ice files" FORCE)

#MESSAGE( STATUS "DEBUG: XNS_SLICE_SOURCE_FILES="${slice_source} )
#MESSAGE( STATUS "DEBUG: XNS_SLICE_SOURCE_FILES="${XNS_UTILSLICE_SOURCE_FILES} )

#
# C++
#
MESSAGE( STATUS "Planning to build C++ version of Slice interfaces ${SEARCH_RELATION_SLICE_LIB_NAME}." )

INCLUDE( ${XNS_CMAKE_DIR}/slice2cpp_rules.cmake )

GENERATE_SLICE2CPP_RULES( ${SEARCHRELATIONSLICE_SRC_DIR} ${SEARCHRELATIONSLICE_CPP_DIR} relation_slice_generated_sources
slice_generated_headers ${XNS_SEARCHRELATIONSLICE_SOURCE_FILES} )
#MESSAGE( STATUS "DEBUG: build_interface_lib_cpp.cmake: slice_generated_headers=${slice_generated_headers}" )
#MESSAGE( STATUS "DEBUG: build_interface_lib_cpp.cmake: slice_generated_sources=${slice_generated_sources}" )
#SET( XNS_SLICE_GENERATED_HEADERS ${slice_generated_headers} CACHE INTERNAL "C++ source files generated from Slice sources" FORCE )
INCLUDE_DIRECTORIES( ${SEARCHRELATIONSLICE_CPP_DIR} )

ADD_LIBRARY( SearchRelationSlice ${relation_slice_generated_sources} )

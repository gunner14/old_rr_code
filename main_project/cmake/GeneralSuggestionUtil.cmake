
FILE( GLOB BUILD_SRC_CPP_SOURCE "${XNS_ROOT_DIR}/search/GeneralSuggestion2/util/*" )

ADD_LIBRARY( "GeneralSuggestionUtil" ${BUILD_SRC_CPP_SOURCE})


FILE( GLOB BUILD_TRIPODCONFIGCLIENT_SRC_CPP_SOURCE "${XNS_ROOT_DIR}/tripod-new/src/cpp/configclient/*.cpp" )

GENERATE_ARCHIVE_EACH( TripodConfigClient "${BUILD_TRIPODCONFIGCLIENT_SRC_CPP_SOURCE}")

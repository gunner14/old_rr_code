
FILE( GLOB BUILD_SRC_CPP_SOURCE "${XNS_ROOT_DIR}/OceCxxAdapter/src/*.cpp" )

GENERATE_ARCHIVE_EACH( OceCxxAdapter "${BUILD_SRC_CPP_SOURCE}")


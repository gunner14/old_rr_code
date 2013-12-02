
FILE(GLOB CLUSTERSTATE_SERVER_SOURCE "${XNS_ROOT_DIR}/ReplicaCluster/ClusterState/src/server/*.cpp")
FILE(GLOB CLUSTERSTATE_CLIENT_SOURCE "${XNS_ROOT_DIR}/ReplicaCluster/ClusterState/src/client/cxx/*.cpp")

FILE(GLOB SERVERSTATE_SERVER_SOURCE "${XNS_ROOT_DIR}/ReplicaCluster/ServerState/src/server/*.cpp")
FILE(GLOB SERVERSTATE_CLIENT_SOURCE "${XNS_ROOT_DIR}/ReplicaCluster/ServerState/src/client/cxx/*.cpp")

FILE(GLOB SO_SERVER_SOURCE "${XNS_ROOT_DIR}/ReplicaCluster/SubjectObserver/src/server/*.cpp")
FILE(GLOB SO_CLIENT_SOURCE "${XNS_ROOT_DIR}/ReplicaCluster/SubjectObserver/src/client/cxx/*.cpp")

SET(REPLICA_CLUSTER_SOURCE ${CLUSTERSTATE_SERVER_SOURCE} ${CLUSTERSTATE_CLIENT_SOURCE} ${SERVERSTATE_SERVER_SOURCE} ${SERVERSTATE_CLIENT_SOURCE} ${SO_SERVER_SOURCE} ${SO_CLIENT_SOURCE})

ADD_LIBRARY(ReplicaCluster STATIC ${REPLICA_CLUSTER_SOURCE})


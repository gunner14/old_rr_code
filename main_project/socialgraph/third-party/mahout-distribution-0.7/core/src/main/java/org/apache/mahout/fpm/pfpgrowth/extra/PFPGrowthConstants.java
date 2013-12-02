package org.apache.mahout.fpm.pfpgrowth.extra;

public interface PFPGrowthConstants {

    // jvm 
    String MAPRED_MAP_CHILD_JAVA_OPTS = "-Xmx4096m -XX:+UseConcMarkSweepGC";
    String MAPRED_REDUCE_CHILD_JAVA_OPTS = "-Xmx2048m -XX:+UseConcMarkSweepGC";
    //String MAPRED_CHILD_JAVA_OPTS = "-Xmx4096m -XX:+UseConcMarkSweepGC";
    int MAPRED_JOB_JVM_NUMTASKS = 3;

    // map task (the smaller, the more tasks)
    int MAPRED_MAX_SPLIT_SIZE = 48 * 1024 * 1024;

    // reduce task
    int MAPRED_REDUCE_PARALLEL_COPIES = 8;
    int NUM_REDUCE_TASKS_PARALLEL_COUNTING = 500;
    int NUM_REDUCE_TASKS_PARALLEL_FPGROWTH = 3000;
    int NUM_REDUCE_TASKS_AGGREGATEING = 1000;
    int NUM_REDUCE_TASKS_GENE_ASSOCIATION_RULES = 2000;
    int NUM_REDUCE_TASKS_AGGREGATING_RULES = 3000;
    int NUM_REDUCE_TASKS_ASSIGN_RULE_ID = 0;
    int NUM_REDUCE_TASKS_GENE_RULE_INDEX = 3000;

    // shuffle and sort 
    int IO_SORT_MB = 128;
    int IO_SORT_FACTOR = 70;
    int IO_FILE_BUFFER_SIZE = 256 * 1024;
    float MAPRED_JOB_SHUFFLE_INPUT_BUFFER_PERCENT = 0.85f;
    float MAPRED_JOB_SHUFFLE_MERGE_PERCENT = 0.8f;
    int MAPRED_TT_HTTP_THREADS = 25;

    // other
    int MAPRED_TASK_TIMEOUT = 6 * 3600 * 1000;

    /* distribute libraries */
    String LIB_JAR_BASE_DIR = "/user/xiang.zhang/pfpgrowth-libjars/";
    String[] LIB_JARS = { "tripod-cache-client-1.0-SNAPSHOT.jar", "tripod-core-1.0-SNAPSHOT.jar",
            "commons-logging-1.1.1.jar", "log4j-1.2.9.jar", "zookeeper-3.3.3-961-patched.jar", "jedis-2.0.0.jar",
            "commons-pool-1.5.6.jar", "Ice-3.3.1-PATCHED.jar", "oce-slice-1.0-SNAPSHOT.jar",
            "oce-proto-1.0-SNAPSHOT.jar", "protobuf-java-2.3.0.jar", "oce-logger-1.0-SNAPSHOT.jar",
            "oce-util-1.0-SNAPSHOT.jar", "oce-ha-1.0-SNAPSHOT.jar", "socialgraph-proto-1.0-SNAPSHOT.jar" };
}

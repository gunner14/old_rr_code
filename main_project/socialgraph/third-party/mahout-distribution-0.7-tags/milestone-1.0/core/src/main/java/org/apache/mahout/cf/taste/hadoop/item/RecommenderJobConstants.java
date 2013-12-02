package org.apache.mahout.cf.taste.hadoop.item;

public interface RecommenderJobConstants {

	// jvm
	String MAPRED_MAP_CHILD_JAVA_OPTS = "-Xmx4096m -XX:+UseConcMarkSweepGC";
	String MAPRED_REDUCE_CHILD_JAVA_OPTS = "-Xmx2048m -XX:+UseConcMarkSweepGC";
	/*String MAPRED_CHILD_JAVA_OPTS = "-Xmx2048m -XX:+UseConcMarkSweepGC";*/
	int MAPRED_JOB_JVM_NUMTASKS = 3;
	
	// map task
	int MAPRED_MAX_SPLIT_SIZE = 32 * 1024 * 1024;
	
	// reduce site
	int MAPRED_REDUCE_PARALLEL_COPIES = 8;
	int NUM_REDUCE_TASKS_PREPARE = 500;
	int NUM_REDUCE_TASKS_ROW_SIMILARITY = 3000;
	int NUM_REDUCE_TASKS_AGGREGATE_AND_RECOMMEND = 3000;
	
	// shuffle and sort 
	int IO_SORT_MB = 128;
    int IO_SORT_FACTOR = 70;
    int IO_FILE_BUFFER_SIZE = 128 * 1024;
	float MAPRED_JOB_SHUFFLE_INPUT_BUFFER_PERCENT = 0.85f;
    float MAPRED_JOB_SHUFFLE_MERGE_PERCENT = 0.8f;
	int MAPRED_TT_HTTP_THREADS = 10;
	
	// other
    int MAPRED_TASK_TIMEOUT = 90 * 60 * 1000;
	
}

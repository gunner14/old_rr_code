package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.filecache.DistributedCache;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapred.JobPriority;

public final class PFPGrowthJobOptimizer {

    /**
     * Modify the job configurations,to get high perfermance.
     * @param jobConf
     */
    public static void optimizeJob(Configuration jobConf) {
        if (jobConf == null) {
            return;
        }

        jobConf.setBoolean("mapreduce.map.speculative", false);
        jobConf.setBoolean("mapreduce.reduce.speculative", false);
        jobConf.setEnum("mapred.job.priority", JobPriority.HIGH);
        jobConf.setBoolean("mapred.compress.map.output", true);
        jobConf.set("mapred.output.compression.type", "BLOCK");

        jobConf.set("mapreduce.map.java.opts", PFPGrowthConstants.MAPRED_MAP_CHILD_JAVA_OPTS);
        jobConf.set("mapreduce.reduce.java.opts", PFPGrowthConstants.MAPRED_REDUCE_CHILD_JAVA_OPTS);
        jobConf.setInt("mapreduce.job.jvm.numtasks", PFPGrowthConstants.MAPRED_JOB_JVM_NUMTASKS);

        jobConf.setInt("mapred.max.split.size", PFPGrowthConstants.MAPRED_MAX_SPLIT_SIZE);

        jobConf.setInt("mapreduce.reduce.shuffle.parallelcopies", PFPGrowthConstants.MAPRED_REDUCE_PARALLEL_COPIES);
        jobConf.setInt("mapreduce.task.io.sort.mb", PFPGrowthConstants.IO_SORT_MB);
        jobConf.setInt("mapreduce.task.io.sort.factor", PFPGrowthConstants.IO_SORT_FACTOR);
        jobConf.setInt("io.file.buffer.size", PFPGrowthConstants.IO_FILE_BUFFER_SIZE);

        jobConf.setFloat("mapred.job.shuffle.input.buffer.percent",
                PFPGrowthConstants.MAPRED_JOB_SHUFFLE_INPUT_BUFFER_PERCENT);
        jobConf.setFloat("mapred.job.shuffle.merge.percent", PFPGrowthConstants.MAPRED_JOB_SHUFFLE_MERGE_PERCENT);
        jobConf.setInt("mapreduce.tasktracker.http.threads", PFPGrowthConstants.MAPRED_TT_HTTP_THREADS);

        jobConf.setInt("mapreduce.task.timeout", PFPGrowthConstants.MAPRED_TASK_TIMEOUT);
    }

    /**
     * Add distributed libraries for the specified job.
     * 
     * @param libJarBasePath
     * @param libJars
     * @param conf job configuration
     */
    public static void addLibJars(String libJarBasePath, String[] libJars, Configuration conf) {
        for (String libJar : libJars) {
            Path libJarPath = new Path(libJarBasePath + libJar);
            try {
                DistributedCache.addArchiveToClassPath(libJarPath, conf);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

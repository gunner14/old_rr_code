package com.renren.datamining.friendsoffriends;

import org.apache.hadoop.io.SequenceFile.CompressionType;
import org.apache.hadoop.io.compress.CompressionCodec;
import org.apache.hadoop.io.compress.SnappyCodec;
import org.apache.hadoop.mapreduce.Job;

public class JobUtils {

    @Deprecated
    public static void InitJobOptimizations(Job job, String jobQueue) {
        System.out.println("[JobUtils] This class is deprecated. User general-settings.xml instead.");

        job.getConfiguration().set("mapred.child.java.opts", "-Xmx10000m -XX:+UseConcMarkSweepGC -verbose:gc -Xloggc:/tmp/@taskid@.gc");
        // job.getConfiguration().setInt("mapred.job.reuse.jvm.num.tasks", 10);
        job.getConfiguration().setBoolean("ipc.client.tcpnodelay", true);
        job.getConfiguration().setInt("ipc.ping.interval", 3);

        job.getConfiguration().set("mapred.map.tasks.speculative.execution", "true");
        job.getConfiguration().set("mapred.reduce.tasks.speculative.execution", "true");
        job.getConfiguration().setInt("mapred.map.max.attempts", 1000000);
        job.getConfiguration().setInt("mapred.max.map.failures.percent", 100);
        job.getConfiguration().setInt("mapred.reduce.max.attempts", 1000000);
        job.getConfiguration().setInt("mapred.max.reduce.failures.percent", 100);
        job.getConfiguration().setInt("mapred.task.timeout", 3600000);
        job.getConfiguration().setInt("mapred.max.tracker.failures", 10000);
        job.getConfiguration().setInt("mapred.min.split.size", 6 * 134217728);

        job.getConfiguration().setBoolean("mapred.compress.map.output", true);
        job.getConfiguration().setEnum("mapred.output.compression.type", CompressionType.RECORD);
        job.getConfiguration().setClass("mapred.output.compression.codec", SnappyCodec.class, CompressionCodec.class);
        job.getConfiguration().set("io.serializations", "org.apache.hadoop.io.serializer.JavaSerialization," 
                                                      + "org.apache.hadoop.io.serializer.WritableSerialization");

        job.getConfiguration().setInt("dfs.namenode.handler.count", 100);
        job.getConfiguration().setInt("dfs.block.size", 268435456);
        job.getConfiguration().setInt("dfs.replication.interval", 10);
        job.getConfiguration().setInt("tasktracker.http.threads", 50);

        job.getConfiguration().setFloat("mapred.job.shuffle.input.buffer.percent", 0.85F);
        job.getConfiguration().setFloat("mapred.job.shuffle.merge.percent", 0.8F);

        job.getConfiguration().setInt("io.sort.mb", 512);
        job.getConfiguration().setInt("io.sort.factor", 100);
        job.getConfiguration().setInt("io.file.buffer.size", 262144);
        job.getConfiguration().setInt("mapred.reduce.parallel.copies", 10);

        // HBase Optimizations
        job.getConfiguration().setInt("hbase.regionserver.lease.period", 180000);
        job.getConfiguration().setInt("hbase.rpc.timeout", 180000);
        job.getConfiguration().setInt("hbase.client.pause", 20);
        job.getConfiguration().setInt("hbase.client.retries.number", 5000);
        job.getConfiguration().setInt("hbase.hregion.majorcompaction", 0);
        job.getConfiguration().setLong("hbase.hregion.max.filesize", 512 * 1024 * 1024L);
        // job.getConfiguration().setFloat("hfile.block.cache.size", 0.1F);
        job.getConfiguration().setFloat("hbase.regionserver.global.memstore.lowerLimit", 0.4F);
        job.getConfiguration().setFloat("hbase.regionserver.global.memstore.upperLimit", 0.65F);
        job.getConfiguration().setBoolean("hbase.hregion.memstore.mslab.enabled", true);

        if (jobQueue.length() > 0) {
            job.getConfiguration().set("mapred.queue.names", jobQueue);
            job.getConfiguration().set("mapred.job.queue.name", jobQueue);
        }
    }

}

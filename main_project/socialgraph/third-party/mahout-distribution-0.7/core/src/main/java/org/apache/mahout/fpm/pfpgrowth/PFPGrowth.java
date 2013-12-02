/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.mahout.fpm.pfpgrowth;

import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.filecache.DistributedCache;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.mahout.common.HadoopUtil;
import org.apache.mahout.common.Pair;
import org.apache.mahout.common.Parameters;
import org.apache.mahout.common.iterator.sequencefile.PathType;
import org.apache.mahout.common.iterator.sequencefile.SequenceFileDirIterable;
import org.apache.mahout.common.iterator.sequencefile.SequenceFileIterable;
import org.apache.mahout.fpm.pfpgrowth.convertors.string.TopKStringPatterns;
import org.apache.mahout.fpm.pfpgrowth.extra.AggregatingRulesCombiner;
import org.apache.mahout.fpm.pfpgrowth.extra.AggregatingRulesMapper;
import org.apache.mahout.fpm.pfpgrowth.extra.AggregatingRulesReducer;
import org.apache.mahout.fpm.pfpgrowth.extra.ArrayWritable;
import org.apache.mahout.fpm.pfpgrowth.extra.AssignRuleIDMapper;
import org.apache.mahout.fpm.pfpgrowth.extra.AssociationRule;
import org.apache.mahout.fpm.pfpgrowth.extra.CacheUtils;
import org.apache.mahout.fpm.pfpgrowth.extra.GeneAssociationRuleCombiner;
import org.apache.mahout.fpm.pfpgrowth.extra.GeneAssociationRuleMapper;
import org.apache.mahout.fpm.pfpgrowth.extra.GeneAssociationRuleReducer;
import org.apache.mahout.fpm.pfpgrowth.extra.GeneRuleIndexMapper;
import org.apache.mahout.fpm.pfpgrowth.extra.GeneRuleIndexReducer;
import org.apache.mahout.fpm.pfpgrowth.extra.PFPGrowthConstants;
import org.apache.mahout.fpm.pfpgrowth.extra.PFPGrowthJobOptimizer;
import org.apache.mahout.fpm.pfpgrowth.extra.PFPGrowthUtils;
import org.apache.mahout.fpm.pfpgrowth.extra.RuleIDAndWeightWritable;
import org.apache.mahout.fpm.pfpgrowth.extra.TopKAssociationRules;
import org.apache.mahout.fpm.pfpgrowth.extra.UploadAssociationRulesMapper;
import org.apache.mahout.fpm.pfpgrowth.extra.UploadRuleIndexMapper;
import org.apache.mahout.fpm.pfpgrowth.fpgrowth.FPGrowth;
import org.apache.mahout.math.list.IntArrayList;

import com.google.common.collect.Lists;

/**
 * 
 * Parallel FP Growth Driver Class. Runs each stage of PFPGrowth as described in the paper
 * http://infolab.stanford.edu/~echang/recsys08-69.pdf
 * 
 */
public final class PFPGrowth {

    public static final String ENCODING = "encoding";
    public static final String F_LIST = "fList";
    public static final String F_LIST_SIZE = "fListSize";
    public static final String G_LIST = "gList";
    public static final String NUM_GROUPS = "numGroups";
    public static final int NUM_GROUPS_DEFAULT = 1000;
    public static final String MAX_PER_GROUP = "maxPerGroup";
    public static final String OUTPUT = "output";
    public static final String MIN_SUPPORT = "minSupport";
    public static final String MAX_HEAPSIZE = "maxHeapSize";
    public static final String INPUT = "input";
    public static final String PFP_PARAMETERS = "pfp.parameters";
    public static final String FILE_PATTERN = "part-*";
    public static final String PARALLEL_COUNTING = "parallelcounting";
    public static final String FPGROWTH = "fpgrowth";
    public static final String FREQUENT_PATTERNS = "frequentpatterns";
    public static final String GENE_ASSOCIATION_RULES = "geneAssociationRules";
    public static final String AGGREGATING_RULES = "aggregatingRules";
    public static final String NUM_ASSOCIATION_RULES = "numAssociationRules";
    public static final String ASSIGN_RULE_ID = "assignRuleID";
    public static final String GENE_RULE_INDEX = "geneRuleIndex";
    public static final String UPLOAD_ASSOCIATION_RULES = "uploadAssociationRules";
    public static final String UPLOAD_RULE_INDEX = "uploadRuleIndex";
    public static final String SPLIT_PATTERN = "splitterPattern"/*"splitPattern"*/;
    public static final String MIN_CONFIDENCE = "minConfidence";
    public static final String MAX_RULES_PER_USER = "maxRulesPerUser";
    public static final String MAX_RULES_PER_USER_GENE_FROM_FP = "maxRulesPerUserGeneFromFP";
    public static final String USE_FPG2 = "use_fpg2";
    public static final Pattern SPLITTER = Pattern.compile("[ ,\t]*[,|\t][ ,\t]*");
    public static final String MIN_FP_LEN = "minFPLen";
    public static final String MAP_TASK_TIMEOUT = "mapTaskTimeout";
    public static final String REDUCE_TASK_TIMEOUT = "reduceTaskTimeout";

    private PFPGrowth() {
    }

    public static List<Pair<Integer, Integer>> readFList2(Configuration conf) throws IOException {
        List<Pair<Integer, Integer>> list = new ArrayList<Pair<Integer, Integer>>();

        Path[] files = DistributedCache.getLocalCacheFiles(conf);
        if (files == null) {
            throw new IOException("Cannot read Frequency list from Distributed Cache");
        }
        if (files.length != 1) {
            throw new IOException("Cannot read Frequency list from Distributed Cache (" + files.length + ")");
        }
        FileSystem fs = FileSystem.getLocal(conf);
        Path fListLocalPath = fs.makeQualified(files[0]);
        // Fallback if we are running locally.
        if (!fs.exists(fListLocalPath)) {
            URI[] filesURIs = DistributedCache.getCacheFiles(conf);
            if (filesURIs == null) {
                throw new IOException("Cannot read Frequency list from Distributed Cache");
            }
            if (filesURIs.length != 1) {
                throw new IOException("Cannot read Frequency list from Distributed Cache (" + files.length + ")");
            }
            fListLocalPath = new Path(filesURIs[0].getPath());
        }

        for (Pair<Text, LongWritable> record : new SequenceFileIterable<Text, LongWritable>(fListLocalPath, true, conf)) {
            list.add(new Pair<Integer, Integer>(Integer.parseInt(record.getFirst().toString()), (int) record
                    .getSecond().get()));
        }
        return list;
    }

    /**
     * Generates the fList from the serialized string representation
     * 
     * @return Deserialized Feature Frequency List
     */
    public static List<Pair<String, Long>> readFList(Configuration conf) throws IOException {
        List<Pair<String, Long>> list = new ArrayList<Pair<String, Long>>();

        Path[] files = DistributedCache.getLocalCacheFiles(conf);
        if (files == null) {
            throw new IOException("Cannot read Frequency list from Distributed Cache");
        }
        if (files.length != 1) {
            throw new IOException("Cannot read Frequency list from Distributed Cache (" + files.length + ")");
        }
        FileSystem fs = FileSystem.getLocal(conf);
        Path fListLocalPath = fs.makeQualified(files[0]);
        // Fallback if we are running locally.
        if (!fs.exists(fListLocalPath)) {
            URI[] filesURIs = DistributedCache.getCacheFiles(conf);
            if (filesURIs == null) {
                throw new IOException("Cannot read Frequency list from Distributed Cache");
            }
            if (filesURIs.length != 1) {
                throw new IOException("Cannot read Frequency list from Distributed Cache (" + files.length + ")");
            }
            fListLocalPath = new Path(filesURIs[0].getPath());
        }

        for (Pair<Text, LongWritable> record : new SequenceFileIterable<Text, LongWritable>(fListLocalPath, true, conf)) {
            list.add(new Pair<String, Long>(record.getFirst().toString(), record.getSecond().get()));
        }
        return list;
    }

    /**
     * Serializes the fList and returns the string representation of the List
     * 
     * @return Serialized String representation of List
     */
    public static void saveFList(Iterable<Pair<String, Long>> flist, Parameters params, Configuration conf)
            throws IOException {
        Path flistPath = new Path(params.get(OUTPUT), F_LIST);
        FileSystem fs = FileSystem.get(flistPath.toUri(), conf);
        flistPath = fs.makeQualified(flistPath);
        HadoopUtil.delete(conf, flistPath);
        SequenceFile.Writer writer = new SequenceFile.Writer(fs, conf, flistPath, Text.class, LongWritable.class);
        try {
            for (Pair<String, Long> pair : flist) {
                writer.append(new Text(pair.getFirst()), new LongWritable(pair.getSecond()));
            }
        } finally {
            writer.close();
        }
        DistributedCache.addCacheFile(flistPath.toUri(), conf);
    }

    public static void saveFList2(Iterable<Pair<Integer, Integer>> flist, Parameters params, Configuration conf)
            throws IOException {
        Path flistPath = new Path(params.get(OUTPUT), F_LIST);
        FileSystem fs = FileSystem.get(flistPath.toUri(), conf);
        flistPath = fs.makeQualified(flistPath);
        HadoopUtil.delete(conf, flistPath);
        SequenceFile.Writer writer = new SequenceFile.Writer(fs, conf, flistPath, IntWritable.class, IntWritable.class);
        try {
            for (Pair<Integer, Integer> pair : flist) {
                writer.append(new IntWritable(pair.getFirst()), new IntWritable(pair.getSecond()));
            }
        } finally {
            writer.close();
        }
        DistributedCache.addCacheFile(flistPath.toUri(), conf);
    }

    /**
     * read the feature frequency List which is built at the end of the Parallel counting job
     * 
     * @return Feature Frequency List
     */
    public static List<Pair<String, Long>> readFList(Parameters params) {
        int minSupport = Integer.valueOf(params.get(MIN_SUPPORT, "3"));
        Configuration conf = new Configuration();

        Path parallelCountingPath = new Path(params.get(OUTPUT), PARALLEL_COUNTING);

        PriorityQueue<Pair<String, Long>> queue = new PriorityQueue<Pair<String, Long>>(11,
                new Comparator<Pair<String, Long>>() {

                    @Override
                    public int compare(Pair<String, Long> o1, Pair<String, Long> o2) {
                        int ret = o2.getSecond().compareTo(o1.getSecond());
                        if (ret != 0) {
                            return ret;
                        }
                        return o1.getFirst().compareTo(o2.getFirst());
                    }
                });

        for (Pair<Text, LongWritable> record : new SequenceFileDirIterable<Text, LongWritable>(new Path(
                parallelCountingPath, FILE_PATTERN), PathType.GLOB, null, null, true, conf)) {
            ////
            if (Boolean.parseBoolean(params.get("debug"))) {
                System.out.printf("read FList record: %s%n", record.toString());
            }
            ////
            long value = record.getSecond().get();
            if (value >= minSupport) {
                queue.add(new Pair<String, Long>(record.getFirst().toString(), value));
            }
        }

        List<Pair<String, Long>> fList = Lists.newArrayList();
        while (!queue.isEmpty()) {
            fList.add(queue.poll());
        }
        return fList;
    }

    public static List<Pair<Integer, Integer>> readFList2(Parameters params) {
        int minSupport = Integer.valueOf(params.get(MIN_SUPPORT, "3"));
        Configuration conf = new Configuration();

        Path parallelCountingPath = new Path(params.get(OUTPUT), PARALLEL_COUNTING);

        PriorityQueue<Pair<String, Long>> queue = new PriorityQueue<Pair<String, Long>>(11,
                new Comparator<Pair<String, Long>>() {

                    @Override
                    public int compare(Pair<String, Long> o1, Pair<String, Long> o2) {
                        int ret = o2.getSecond().compareTo(o1.getSecond());
                        if (ret != 0) {
                            return ret;
                        }
                        return o1.getFirst().compareTo(o2.getFirst());
                    }
                });

        for (Pair<Text, LongWritable> record : new SequenceFileDirIterable<Text, LongWritable>(new Path(
                parallelCountingPath, FILE_PATTERN), PathType.GLOB, null, null, true, conf)) {
            long value = record.getSecond().get();
            if (value >= minSupport) {
                queue.add(new Pair<String, Long>(record.getFirst().toString(), value));
            }
        }
        List<Pair<Integer, Integer>> fList = Lists.newArrayList();
        while (!queue.isEmpty()) {
            Pair<String, Long> item = queue.poll();
            fList.add(new Pair<Integer, Integer>(Integer.parseInt(item.getFirst()), item.getSecond().intValue()));
        }
        return fList;
    }

    public static int getGroup(int itemId, int maxPerGroup) {
        return itemId / maxPerGroup;
    }

    public static IntArrayList getGroupMembers(int groupId, int maxPerGroup, int numFeatures) {
        IntArrayList ret = new IntArrayList();
        int start = groupId * maxPerGroup;
        int end = start + maxPerGroup;
        if (end > numFeatures) end = numFeatures;
        for (int i = start; i < end; i++) {
            ret.add(i);
        }
        return ret;
    }

    /**
     * Read the Frequent Patterns generated from Text
     * 
     * @return List of TopK patterns for each string frequent feature
     */
    public static List<Pair<String, TopKStringPatterns>> readFrequentPattern(Parameters params) throws IOException {

        Configuration conf = new Configuration();

        Path frequentPatternsPath = new Path(params.get(OUTPUT), FREQUENT_PATTERNS);
        FileSystem fs = FileSystem.get(frequentPatternsPath.toUri(), conf);
        FileStatus[] outputFiles = fs.globStatus(new Path(frequentPatternsPath, FILE_PATTERN));

        List<Pair<String, TopKStringPatterns>> ret = Lists.newArrayList();
        for (FileStatus fileStatus : outputFiles) {
            ret.addAll(FPGrowth.readFrequentPattern(conf, fileStatus.getPath()));
        }
        return ret;
    }

    /**
     * 
     * @param params params should contain input and output locations as a string value, the
     *        additional parameters include minSupport(3), maxHeapSize(50), numGroups(1000)
     */
    public static void runPFPGrowth(Parameters params) throws IOException, InterruptedException, ClassNotFoundException {
        Configuration conf = new Configuration();
        conf.set("io.serializations", "org.apache.hadoop.io.serializer.JavaSerialization,"
                + "org.apache.hadoop.io.serializer.WritableSerialization");
        AtomicInteger currentPhase = new AtomicInteger();

        if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            startParallelCounting(params, conf);
        }

        if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            // save feature list to dcache
            System.out.println("reading fList...");
            List<Pair<String, Long>> fList = readFList(params);
            saveFList(fList, params, conf);

            // save fList size
            HadoopUtil.writeInt(fList.size(), new Path(params.get(OUTPUT), F_LIST_SIZE), new Configuration());
            System.out.println("fList size:" + fList.size());

            // set param to control group size in MR jobs
            int numGroups = params.getInt(PFPGrowth.NUM_GROUPS, PFPGrowth.NUM_GROUPS_DEFAULT);
            int maxPerGroup = fList.size() / numGroups;
            if (fList.size() % numGroups != 0) maxPerGroup++;
            params.set(MAX_PER_GROUP, Integer.toString(maxPerGroup));
            System.out.println("maxNumItemPerGroup:" + maxPerGroup);
            fList = null;

            startParallelFPGrowth(params, conf);
        }

        if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            startAggregating(params, conf);
        }

        // generate association rules from FP
        if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            startGeneAssociateRules(params, conf);
        }

        // assign ID for each rule
        if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            startAssignRuleID(params, conf);
        }

        // aggregating and generate the final rule list for each user
        if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            startAggregatingRules(params, conf);
        }

        /*
        if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            startCreateRuleIndex(params, conf);
        }*/

        /* upload assoication rules and index to cache */
        /*if (PFPGrowthUtils.shouldRunNextPhase(params, currentPhase)) {
            startUploadData(params, conf);
        }*/
    }

    /**
     * Upload assoication rules and index to cache
     * 
     * @param params
     * @param conf
     */
    private static void startUploadData(final Parameters params, final Configuration conf) {
        try {
            startUploadAssociationRules(params, conf);
        } catch (Exception e) {
            e.printStackTrace();
        }

        /*ExecutorService executorService = Executors.newFixedThreadPool(2);

        executorService.submit(new Runnable() {

            @Override
            public void run() {
                try {
                    startUploadAssociationRules(params, conf);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        executorService.submit(new Runnable() {

            @Override
            public void run() {
                try {
                    startUploadRuleIndex(params, conf);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        executorService.shutdown();*/
    }

    private static void startUploadRuleIndex(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(OUTPUT), GENE_RULE_INDEX);

        Job job = new Job(conf, "PFP upload rules index to cache running over input: " + input);
        job.setJarByClass(PFPGrowth.class);

        job.setMapOutputKeyClass(NullWritable.class);
        job.setMapOutputValueClass(NullWritable.class);
        job.setOutputKeyClass(NullWritable.class);
        job.setOutputValueClass(NullWritable.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), UPLOAD_RULE_INDEX);
        FileOutputFormat.setOutputPath(job, outPath);

        //job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(UploadRuleIndexMapper.class);
        //job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setNumReduceTasks(0);
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());
        PFPGrowthJobOptimizer.addLibJars(PFPGrowthConstants.LIB_JAR_BASE_DIR, PFPGrowthConstants.LIB_JARS, job
                .getConfiguration());
        //job.getConfiguration().setInt("mapred.max.split.size", 32 * 1024 * 1024);

        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }
    }

    private static void startUploadAssociationRules(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(OUTPUT), AGGREGATING_RULES);

        /* debug only */
        if (params.get("cacheInput") != null) {
            input = new Path(params.get(OUTPUT), params.get("cacheInput"));
        }

        Job job = new Job(conf, "PFP upload association rules to cache running over input: " + input);
        job.setJarByClass(PFPGrowth.class);

        job.setMapOutputKeyClass(NullWritable.class);
        job.setMapOutputValueClass(NullWritable.class);
        job.setOutputKeyClass(NullWritable.class);
        job.setOutputValueClass(NullWritable.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), UPLOAD_ASSOCIATION_RULES);
        FileOutputFormat.setOutputPath(job, outPath);

        //job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(UploadAssociationRulesMapper.class);
        //job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setNumReduceTasks(0);
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());
        PFPGrowthJobOptimizer.addLibJars(PFPGrowthConstants.LIB_JAR_BASE_DIR, PFPGrowthConstants.LIB_JARS, job
                .getConfiguration());
        //job.getConfiguration().setInt("mapred.min.split.size", 1024 * 1024 * 1024);

        HadoopUtil.delete(conf, outPath);
        CacheUtils.resetStat();
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }
        System.out.println(CacheUtils.getSummaryStat());
    }

    private static void startCreateRuleIndex(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(OUTPUT), ASSIGN_RULE_ID);

        Job job = new Job(conf, "6/6 PFP generate rule index running over input: " + input);
        job.setJarByClass(PFPGrowth.class);

        job.setMapOutputKeyClass(LongWritable.class);
        job.setMapOutputValueClass(RuleIDAndWeightWritable.class);
        job.setOutputKeyClass(LongWritable.class);
        job.setOutputValueClass(ArrayWritable.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), GENE_RULE_INDEX);
        FileOutputFormat.setOutputPath(job, outPath);

        //job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(GeneRuleIndexMapper.class);
        job.setReducerClass(GeneRuleIndexReducer.class);
        //job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setNumReduceTasks(params.getInt("numGeneRuleIndexReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_GENE_RULE_INDEX));
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());
        job.getConfiguration().setInt("mapred.min.split.size", 256 * 1024 * 1024);

        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }
    }

    /**
     * Run the aggregating-rules Job to generate topK association rules for each item.
     * 
     * @param params
     * @param conf
     * @throws IOException
     * @throws InterruptedException
     * @throws ClassNotFoundException
     */
    private static void startAggregatingRules(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(OUTPUT), ASSIGN_RULE_ID);

        Job job = new Job(conf, "6/6 AggregatingAssociationRules");
        job.setJarByClass(PFPGrowth.class);

        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(TopKAssociationRules.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), AGGREGATING_RULES);
        FileOutputFormat.setOutputPath(job, outPath);

        //job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(AggregatingRulesMapper.class);
        job.setCombinerClass(AggregatingRulesCombiner.class);
        job.setReducerClass(AggregatingRulesReducer.class);
        //job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setNumReduceTasks(params.getInt("numAggregatingRulesReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_AGGREGATING_RULES));
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());

        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }
    }

    private static void startAssignRuleID(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(OUTPUT), GENE_ASSOCIATION_RULES);

        Job job = new Job(conf, "5/6 PFP Assign ruleID");
        job.setJarByClass(PFPGrowth.class);

        job.setOutputKeyClass(NullWritable.class);
        job.setOutputValueClass(AssociationRule.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), ASSIGN_RULE_ID);
        FileOutputFormat.setOutputPath(job, outPath);

        //job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(AssignRuleIDMapper.class);
        //job.setReducerClass(AssignRuleIDReducer.class);
        //job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        /*job.setNumReduceTasks(params
                .getInt("numAssignRuleIDReduce", PFPGrowthConstants.NUM_REDUCE_TASKS_ASSIGN_RULE_ID));*/
        job.setNumReduceTasks(0);
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());
        job.getConfiguration().setInt("mapred.min.split.size", 256 * 1024 * 1024);

        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }
    }

    /**
     * Run the generating-association-rules Job to derive association rules from frequent itemset.
     * 
     * @param params
     * @param conf
     * @throws IOException
     * @throws InterruptedException
     * @throws ClassNotFoundException
     */
    private static void startGeneAssociateRules(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(OUTPUT), FREQUENT_PATTERNS);

        Job job = new Job(conf, "4/6 GeneAssociationRules");
        job.setJarByClass(PFPGrowth.class);

        job.setMapOutputKeyClass(Text.class);
        job.setOutputKeyClass(NullWritable.class);
        job.setOutputValueClass(AssociationRule.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), GENE_ASSOCIATION_RULES);
        FileOutputFormat.setOutputPath(job, outPath);

        //job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(GeneAssociationRuleMapper.class);
        job.setCombinerClass(GeneAssociationRuleCombiner.class);
        job.setReducerClass(GeneAssociationRuleReducer.class);
        //job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setNumReduceTasks(params.getInt("numGeneAssociationRulesReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_GENE_ASSOCIATION_RULES));
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());

        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }

        long numAssociationRules = job.getCounters().findCounter(
                GeneAssociationRuleReducer.Counters.NUM_ASSOCIATION_RULES).getValue();
        HadoopUtil.writeLong(numAssociationRules, new Path(params.get(OUTPUT), NUM_ASSOCIATION_RULES), job
                .getConfiguration());
        System.out.println("numAssociationRules: " + numAssociationRules);
    }

    /**
     * Run the aggregation Job to aggregate the different TopK patterns and group each Pattern by
     * the features present in it and thus calculate the final Top K frequent Patterns for each
     * feature
     */
    public static void startAggregating(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(OUTPUT), FPGROWTH);

        Job job = new Job(conf, "3/6 AggregatingFP");
        job.setJarByClass(PFPGrowth.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(TopKStringPatterns.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), FREQUENT_PATTERNS);
        FileOutputFormat.setOutputPath(job, outPath);

        job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setMapperClass(AggregatorMapper.class);
        job.setCombinerClass(AggregatorReducer.class);
        job.setReducerClass(AggregatorReducer.class);
        //job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setNumReduceTasks(params.getInt("numAggregateReduce", PFPGrowthConstants.NUM_REDUCE_TASKS_AGGREGATEING));
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());

        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }
    }

    /**
     * Run the Parallel FPGrowth Map/Reduce Job to calculate the Top K features of group dependent
     * shards
     */
    public static void startParallelFPGrowth(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        Path input = new Path(params.get(INPUT));
        Job job = new Job(conf, "2/6 parallelFPGrowth");
        job.setJarByClass(PFPGrowth.class);

        job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(TransactionTree.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(TopKStringPatterns.class);

        FileInputFormat.addInputPath(job, input);
        Path outPath = new Path(params.get(OUTPUT), FPGROWTH);
        FileOutputFormat.setOutputPath(job, outPath);

        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(ParallelFPGrowthMapper.class);
        job.setCombinerClass(ParallelFPGrowthCombiner.class);
        job.setReducerClass(ParallelFPGrowthReducer.class);
        job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setNumReduceTasks(params.getInt("numParallelFPGrowthReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_PARALLEL_FPGROWTH));
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());
        job.getConfiguration().setInt("mapred.max.split.size", 32 * 1024 * 1024);
        job.getConfiguration().set("mapreduce.map.java.opts", "-Xms1024m -Xmx4096m -XX:+UseConcMarkSweepGC");
        job.getConfiguration().setInt("mapreduce.job.jvm.numtasks", 1);
        //job.getConfiguration().setBoolean("mapreduce.map.speculative", true);
        job.getConfiguration().setInt("mapreduce.task.io.sort.mb", 256);
        job.getConfiguration().set("mapreduce.reduce.java.opts", "-Xms1024m -Xmx4096m -XX:+UseConcMarkSweepGC");
        job.getConfiguration().setInt("mapreduce.task.io.sort.factor", 150);
        job.getConfiguration().setFloat("io.sort.spill.percent", 0.75f);
        job.getConfiguration().setInt("mapred.max.map.failures.percent", 5);
        job.getConfiguration().setInt("mapred.max.reduce.failures.percent",5);
        
        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }
    }

    /**
     * Count the frequencies of various features in parallel using Map/Reduce
     */
    public static void startParallelCounting(Parameters params, Configuration conf) throws IOException,
            InterruptedException, ClassNotFoundException {
        conf.set(PFP_PARAMETERS, params.toString());
        String input = params.get(INPUT);

        Job job = new Job(conf, "1/6 ParallelCounting");
        job.setJarByClass(PFPGrowth.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(LongWritable.class);

        FileInputFormat.addInputPath(job, new Path(input));
        Path outPath = new Path(params.get(OUTPUT), PARALLEL_COUNTING);
        FileOutputFormat.setOutputPath(job, outPath);

        job.setInputFormatClass(TextInputFormat.class);
        job.setMapperClass(ParallelCountingMapper.class);
        job.setCombinerClass(ParallelCountingReducer.class);
        job.setReducerClass(ParallelCountingReducer.class);
        job.setOutputFormatClass(SequenceFileOutputFormat.class);
        job.setNumReduceTasks(params.getInt("numParallelCountingReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_PARALLEL_COUNTING));
        PFPGrowthJobOptimizer.optimizeJob(job.getConfiguration());

        HadoopUtil.delete(conf, outPath);
        boolean succeeded = job.waitForCompletion(true);
        if (!succeeded) {
            throw new IllegalStateException("Job failed!");
        }

    }
}

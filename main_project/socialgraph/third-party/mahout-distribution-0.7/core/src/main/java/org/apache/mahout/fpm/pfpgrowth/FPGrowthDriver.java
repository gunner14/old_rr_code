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

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.util.ToolRunner;
import org.apache.mahout.common.AbstractJob;
import org.apache.mahout.common.Pair;
import org.apache.mahout.common.Parameters;
import org.apache.mahout.common.iterator.FileLineIterable;
import org.apache.mahout.common.iterator.StringRecordIterator;
import org.apache.mahout.fpm.pfpgrowth.convertors.ContextStatusUpdater;
import org.apache.mahout.fpm.pfpgrowth.convertors.SequenceFileOutputCollector;
import org.apache.mahout.fpm.pfpgrowth.convertors.string.StringOutputConverter;
import org.apache.mahout.fpm.pfpgrowth.convertors.string.TopKStringPatterns;
import org.apache.mahout.fpm.pfpgrowth.extra.PFPGrowthConstants;
import org.apache.mahout.fpm.pfpgrowth.fpgrowth.FPGrowth;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.io.Closeables;

public final class FPGrowthDriver extends AbstractJob {

    private static final Logger log = LoggerFactory.getLogger(FPGrowthDriver.class);

    private FPGrowthDriver() {
    }

    public static void main(String[] args) throws Exception {
        ToolRunner.run(new Configuration(), new FPGrowthDriver(), args);
    }

    /**
     * Run TopK FPGrowth given the input file,
     */
    @Override
    public int run(String[] args) throws Exception {
        addInputOption();
        addOutputOption();

        addOption("minSupport", "s", "(Optional) The minimum number of times a co-occurrence must be present."
                + " Default Value: 3", "3");
        addOption("maxHeapSize", "k", "(Optional) Maximum Heap Size k, to denote the requirement to mine top K items."
                + " Default value: 50", "50");
        addOption("numGroups", "g",
                "(Optional) Number of groups the features should be divided in the map-reduce version."
                        + " Doesn't work in sequential version Default Value:" + PFPGrowth.NUM_GROUPS_DEFAULT, Integer
                        .toString(PFPGrowth.NUM_GROUPS_DEFAULT));
        addOption("splitterPattern", "regex", "Regular Expression pattern used to split given string transaction into"
                + " itemsets. Default value splits comma separated itemsets.  Default Value:"
                + " \"[ ,\\t]*[,|\\t][ ,\\t]*\" ", "[ ,\t]*[,|\t][ ,\t]*");
        addOption("numTreeCacheEntries", "tc", "(Optional) Number of entries in the tree cache to prevent duplicate"
                + " tree building. (Warning) a first level conditional FP-Tree might consume a lot of memory, "
                + "so keep this value small, but big enough to prevent duplicate tree building. "
                + "Default Value:5 Recommended Values: [5-10]", "5");
        addOption("method", "method", "Method of processing: sequential|mapreduce", "sequential");
        addOption("encoding", "e", "(Optional) The file encoding.  Default value: UTF-8", "UTF-8");
        addFlag("useFPG2", "2", "Use an alternate FPG implementation");
        addOption("minFPLen", "minFPLen", "minimun length of FP, less than this threshold will be omit", "2");
        addOption("numParallelCountingReduce", "numParallelCountingReduce", "numParallelCountingReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_PARALLEL_COUNTING + "");
        addOption("numParallelFPGrowthReduce", "numParallelCountingReduce", "numParallelCountingReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_PARALLEL_FPGROWTH + "");
        addOption("numAggregateReduce", "numParallelCountingReduce", "numParallelCountingReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_AGGREGATEING + "");
        addFlag("debug", "debug", "verbose output");
        addOption("minConfidence", "minConfidence", "minConfidence", "80");
        addOption("maxRulesPerUser", "maxRulesPerUser", "maxRulesPerUser", "100");
        addOption("maxRulesPerUserGeneFromFP", "maxRulesPerUserGeneFromFP", "maxRulesPerUserGeneFromFP", "1000");
        addOption("numGeneAssociationRulesReduce", "numGeneAssociationRulesReduce", "numGeneAssociationRulesReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_GENE_ASSOCIATION_RULES + "");
        addOption("numAssignRuleIDReduce", "numAssignRuleIDReduce", "numAssignRuleIDReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_ASSIGN_RULE_ID + "");
        addOption("numGeneRuleIndexReduce", "numGeneRuleIndexReduce", "numGeneRuleIndexReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_GENE_RULE_INDEX + "");
        addOption("numAggregatingRulesReduce", "numAggregatingRulesReduce", "numAggregatingRulesReduce",
                PFPGrowthConstants.NUM_REDUCE_TASKS_AGGREGATING_RULES + "");

        /* debug only */
        addOption("cacheInput", "cacheInput", "cacheInput", "null");

        addOption("mapTaskTimeout", "mapTaskTimeout", "mapTaskTimeout", 5 * 3600 * 1000 + "");
        addOption("reduceTaskTimeout", "reduceTaskTimeout", "reduceTaskTimeout", 5 * 3600 * 1000 + "");
        
        if (parseArguments(args) == null) {
            return -1;
        }

        Parameters params = new Parameters();

        checkOption(params, "minSupport");
        checkOption(params, "maxHeapSize");
        checkOption(params, "numGroups");
        checkOption(params, "numTreeCacheEntries");
        checkOption(params, "splitterPattern");
        checkOption(params, "encoding");
        checkOption(params, "minFPLen");
        checkOption(params, "numParallelCountingReduce");
        checkOption(params, "numParallelFPGrowthReduce");
        checkOption(params, "numAggregateReduce");
        checkOption(params, "numGeneAssociationRulesReduce");
        checkOption(params, "numAssignRuleIDReduce");
        checkOption(params, "numGeneRuleIndexReduce");
        checkOption(params, "numAggregatingRulesReduce");
        checkOption(params, "minConfidence");
        checkOption(params, "maxRulesPerUser");
        checkOption(params, "maxRulesPerUserGeneFromFP");
        checkOption(params, "startPhase");
        checkOption(params, "endPhase");
        checkOption(params, "mapTaskTimeout");
        checkOption(params, "reduceTaskTimeout");

        if (hasOption("useFPG2")) {
            params.set(PFPGrowth.USE_FPG2, "true");
        }

        if (hasOption("debug")) {
            params.set("debug", "true");
        } else {
            params.set("debug", "false");
        }

        /* debug only */
        if (hasOption("cacheInput")) {
            params.set("cacheInput", getOption("cacheInput"));
        }

        Path inputDir = getInputPath();
        Path outputDir = getOutputPath();

        params.set("input", inputDir.toString());
        params.set("output", outputDir.toString());

        String classificationMethod = getOption("method");
        if ("sequential".equalsIgnoreCase(classificationMethod)) {
            runFPGrowth(params);
        } else if ("mapreduce".equalsIgnoreCase(classificationMethod)) {
            PFPGrowth.runPFPGrowth(params);
        }

        return 0;
    }

    private void checkOption(Parameters params, String key) {
        if (hasOption(key)) {
            params.set(key, getOption(key));
        }
    }

    private static void runFPGrowth(Parameters params) throws IOException {
        log.info("Starting Sequential FPGrowth");
        int maxHeapSize = Integer.valueOf(params.get("maxHeapSize", "50"));
        int minSupport = Integer.valueOf(params.get("minSupport", "3"));

        String output = params.get("output", "output.txt");

        Path path = new Path(output);
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(path.toUri(), conf);

        Charset encoding = Charset.forName(params.get("encoding"));
        String input = params.get("input");

        String pattern = params.get("splitPattern", PFPGrowth.SPLITTER.toString());

        SequenceFile.Writer writer = new SequenceFile.Writer(fs, conf, path, Text.class, TopKStringPatterns.class);

        if ("true".equals(params.get("useFPG2"))) {
            org.apache.mahout.fpm.pfpgrowth.fpgrowth2.FPGrowthObj<String> fp = new org.apache.mahout.fpm.pfpgrowth.fpgrowth2.FPGrowthObj<String>();
            Collection<String> features = new HashSet<String>();

            try {
                fp.generateTopKFrequentPatterns(new StringRecordIterator(new FileLineIterable(new File(input),
                        encoding, false), pattern), fp.generateFList(new StringRecordIterator(new FileLineIterable(
                        new File(input), encoding, false), pattern), minSupport), minSupport, maxHeapSize, features,
                        new StringOutputConverter(new SequenceFileOutputCollector<Text, TopKStringPatterns>(writer)),
                        new ContextStatusUpdater(null));
            } finally {
                Closeables.closeQuietly(writer);
            }
        } else {
            FPGrowth<String> fp = new FPGrowth<String>();
            Collection<String> features = new HashSet<String>();
            try {
                fp.generateTopKFrequentPatterns(new StringRecordIterator(new FileLineIterable(new File(input),
                        encoding, false), pattern), fp.generateFList(new StringRecordIterator(new FileLineIterable(
                        new File(input), encoding, false), pattern), minSupport), minSupport, maxHeapSize, features,
                        new StringOutputConverter(new SequenceFileOutputCollector<Text, TopKStringPatterns>(writer)),
                        new ContextStatusUpdater(null));
            } finally {
                Closeables.closeQuietly(writer);
            }
        }

        List<Pair<String, TopKStringPatterns>> frequentPatterns = FPGrowth.readFrequentPattern(conf, path);
        for (Pair<String, TopKStringPatterns> entry : frequentPatterns) {
            log.info("Dumping Patterns for Feature: {} \n{}", entry.getFirst(), entry.getSecond());
        }
    }
}

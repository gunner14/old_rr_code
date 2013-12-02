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
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;

import org.apache.commons.lang.mutable.MutableLong;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.mahout.common.HadoopUtil;
import org.apache.mahout.common.Pair;
import org.apache.mahout.common.Parameters;
import org.apache.mahout.fpm.pfpgrowth.convertors.ContextStatusUpdater;
import org.apache.mahout.fpm.pfpgrowth.convertors.ContextWriteOutputCollector;
import org.apache.mahout.fpm.pfpgrowth.convertors.integer.IntegerStringOutputConverter;
import org.apache.mahout.fpm.pfpgrowth.convertors.string.TopKStringPatterns;
import org.apache.mahout.fpm.pfpgrowth.fpgrowth.FPGrowth;
import org.apache.mahout.math.Arrays;
import org.apache.mahout.math.list.IntArrayList;

import com.google.common.collect.Lists;

/**
 * takes each group of transactions and runs Vanilla FPGrowth on it and
 * outputs the the Top K frequent Patterns for each group.
 * 
 */
public class ParallelFPGrowthReducer extends Reducer<IntWritable, TransactionTree, Text, TopKStringPatterns> {
    // old
    //private final List<String> featureReverseMap = Lists.newArrayList();
    // new
    private int[] featureReverseMap = null; // save about 5 times memory
    //private final LongArrayList freqList = new LongArrayList();

    private int maxHeapSize = 50;
    private int minSupport = 3;
    private int numFeatures;
    private int maxPerGroup;
    private boolean useFP2;
    private int minFPLen = 1;
    
    // when this reduce task starts to run
    private long beginTime;
    
    /*
     *  maximum milliseconds this reduce task will run, if exceed this threshld, 
     *  the rest of the input records will be skipped.
     */
    private long taskTimeout = 5 * 3600 * 1000;

    public enum Counter {
        numSkipRecords,
    }

    public boolean isTaskTimeout() {
        if ((System.currentTimeMillis() - beginTime) >= taskTimeout) {
            return true;
        }
        return false;
    }

    private static class IteratorAdapter implements Iterator<Pair<List<Integer>, Long>> {

        private Iterator<Pair<IntArrayList, Long>> innerIter;

        private IteratorAdapter(Iterator<Pair<IntArrayList, Long>> transactionIter) {
            innerIter = transactionIter;
        }

        @Override
        public boolean hasNext() {
            return innerIter.hasNext();
        }

        @Override
        public Pair<List<Integer>, Long> next() {
            Pair<IntArrayList, Long> innerNext = innerIter.next();
            return new Pair(innerNext.getFirst().toList(), innerNext.getSecond());
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException();
        }
    }

    private void handleRecord(IntWritable key, Iterable<TransactionTree> values, Context context) throws IOException{
        TransactionTree cTree = new TransactionTree();
        for (TransactionTree tr : values) {
            for (Pair<IntArrayList, Long> p : tr) {
                cTree.addPattern(p.getFirst(), p.getSecond());
            }
        }

        List<Pair<Integer, Long>> localFList = Lists.newArrayList();
        for (Entry<Integer, MutableLong> fItem : cTree.generateFList().entrySet()) {
            localFList.add(new Pair<Integer, Long>(fItem.getKey(), fItem.getValue().toLong()));
        }

        Collections.sort(localFList, new CountDescendingPairComparator<Integer, Long>());

        if (useFP2) {
            /*org.apache.mahout.fpm.pfpgrowth.fpgrowth2.FPGrowthIds fpGrowth = new org.apache.mahout.fpm.pfpgrowth.fpgrowth2.FPGrowthIds();
            fpGrowth.generateTopKFrequentPatterns(cTree.iterator(), freqList, minSupport, maxHeapSize, PFPGrowth
                    .getGroupMembers(key.get(), maxPerGroup, numFeatures), new IntegerStringOutputConverter(
                    new ContextWriteOutputCollector<IntWritable, TransactionTree, Text, TopKStringPatterns>(context),
                    featureReverseMap),
                    new ContextStatusUpdater<IntWritable, TransactionTree, Text, TopKStringPatterns>(context));*/
            throw new RuntimeException("FP2 is unavaliable now.");
        } else {
            // old
            //FPGrowth<Integer> fpGrowth = new FPGrowth<Integer>();
            // new
            FPGrowth<Integer> fpGrowth = new FPGrowth<Integer>(this);
            fpGrowth.generateTopKFrequentPatterns2(new IteratorAdapter(cTree.iterator()), localFList, minSupport,
                    maxHeapSize, new HashSet<Integer>(PFPGrowth.getGroupMembers(key.get(), maxPerGroup, numFeatures)
                            .toList()), new IntegerStringOutputConverter(
                            new ContextWriteOutputCollector<IntWritable, TransactionTree, Text, TopKStringPatterns>(
                                    context), featureReverseMap),
                    new ContextStatusUpdater<IntWritable, TransactionTree, Text, TopKStringPatterns>(context), minFPLen);
        }
    }
    
    
    @Override
    protected void reduce(IntWritable key, Iterable<TransactionTree> values, Context context) throws IOException {
        if(! isTaskTimeout()){
            handleRecord(key, values, context);
        }else{
            context.getCounter(Counter.numSkipRecords).increment(1);
        }
    }

    private void readFeatureReverseMap(Context context, Parameters params) throws IOException {
        int size = HadoopUtil.readInt(new Path(params.get(PFPGrowth.OUTPUT), PFPGrowth.F_LIST_SIZE),
                context.getConfiguration());
        //System.out.println("ParallelFPGrowthReducer read fListSize:" + size);
        featureReverseMap = new int[size];
        int i = 0;
        for (Pair<Integer, Integer> e : PFPGrowth.readFList2(context.getConfiguration())) {
            featureReverseMap[i++] = e.getFirst().intValue();
        }
        /*System.out.println(String.format("ParallelFPGrowthReducer read records:%d\n fMap:%s", i, 
                Arrays.toString(featureReverseMap)));*/
    }
    
    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
        super.setup(context);
        Parameters params = new Parameters(context.getConfiguration().get(PFPGrowth.PFP_PARAMETERS, ""));

        // old
        /*for (Pair<String, Long> e : PFPGrowth.readFList(context.getConfiguration())) {
            featureReverseMap.add(e.getFirst());
            freqList.add(e.getSecond());
        }*/
        // new
        readFeatureReverseMap(context, params);

        maxHeapSize = Integer.valueOf(params.get(PFPGrowth.MAX_HEAPSIZE, "50"));
        minSupport = Integer.valueOf(params.get(PFPGrowth.MIN_SUPPORT, "3"));
        maxPerGroup = params.getInt(PFPGrowth.MAX_PER_GROUP, 0);
        //numFeatures = featureReverseMap.size();
        numFeatures = featureReverseMap.length;
        useFP2 = "true".equals(params.get(PFPGrowth.USE_FPG2));
        minFPLen = params.getInt("minFPLen", 1);
        
        beginTime = System.currentTimeMillis();
        taskTimeout = Long.parseLong(params.get(PFPGrowth.REDUCE_TASK_TIMEOUT));
    }
}

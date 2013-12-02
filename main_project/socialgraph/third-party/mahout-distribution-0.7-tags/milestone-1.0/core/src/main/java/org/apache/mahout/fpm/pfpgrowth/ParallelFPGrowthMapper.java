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
import java.util.regex.Pattern;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.mahout.common.HadoopUtil;
import org.apache.mahout.common.Pair;
import org.apache.mahout.common.Parameters;
import org.apache.mahout.fpm.pfpgrowth.extra.FixedSizeArrayHashMap;
import org.apache.mahout.math.list.IntArrayList;
import org.apache.mahout.math.set.OpenIntHashSet;

/**
 * maps each transaction to all unique items groups in the transaction. mapper outputs the group id
 * as key and the transaction as value
 * 
 */
public class ParallelFPGrowthMapper extends Mapper<LongWritable, Text, IntWritable, TransactionTree> {

    /* raw */
    //private final OpenObjectIntHashMap<String> fMap = new OpenObjectIntHashMap<String>();
    /* version I */
    //private OpenObjectIntHashMap<Integer> fMap;
    /* version II */
    private FixedSizeArrayHashMap fMap;

    private Pattern splitter;
    private int maxPerGroup;
    private IntWritable wGroupID = new IntWritable();
    private Parameters params;

    // when this map task start to run
    private long beginTime;
    /*
     *  maximum milliseconds this map task will run, if exceed this threshld, 
     *  the rest of the input records will be skipped.
     */
    private long taskTimeout = 6 * 3600 * 1000;

    public enum Counter {
        numSkipRecords,
    }

    private boolean isTaskTimeout() {
        if ((System.currentTimeMillis() - beginTime) >= taskTimeout) {
            return true;
        }
        return false;
    }

    private void handleRecord(LongWritable offset, Text input, Context context) throws IOException,
            InterruptedException {
        String[] items = splitter.split(input.toString());
        OpenIntHashSet itemSet = new OpenIntHashSet();

        for (String item : items) {
            if (!item.trim().isEmpty() && fMap.containsKey(Integer.parseInt(item))) {
                itemSet.add(fMap.get(Integer.parseInt(item)));
            }
        }

        IntArrayList itemArr = new IntArrayList(itemSet.size());
        itemSet.keys(itemArr);
        itemArr.sort();

        /* for debug purpose only */
        /*if (Boolean.parseBoolean(params.get("debug"))) {
            System.out.println(String
                    .format("\nhanle record:%s, sorted index:%s", input.toString(), itemArr.toString()));
        }*/

        OpenIntHashSet groups = new OpenIntHashSet();
        for (int j = itemArr.size() - 1; j >= 0; j--) {
            // generate group dependent shards
            int item = itemArr.get(j);
            int groupID = PFPGrowth.getGroup(item, maxPerGroup);

            if (!groups.contains(groupID)) {
                IntArrayList tempItems = new IntArrayList(j + 1);
                tempItems.addAllOfFromTo(itemArr, 0, j);
                context.setStatus("Parallel FPGrowth: Generating Group Dependent transactions for: " + item);
                wGroupID.set(groupID);
                context.write(wGroupID, new TransactionTree(tempItems, 1L));
                /* for debug purpuse only */
                /*if (Boolean.parseBoolean(params.get("debug"))) {
                    System.out.println(String.format("ParallelFPGrowthMapper <%d, (%s, 1)>", groupID, tempItems
                            .toString()));
                }*/
            }
            groups.add(groupID);

            if (isTaskTimeout()) {
                return;
            }
        }
    }

    @Override
    protected void map(LongWritable offset, Text input, Context context) throws IOException, InterruptedException {
        if (!isTaskTimeout()) {
            handleRecord(offset, input, context);
        } else {
            context.getCounter(Counter.numSkipRecords).increment(1);
        }
    }

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
        super.setup(context);
        params = new Parameters(context.getConfiguration().get(PFPGrowth.PFP_PARAMETERS, ""));

        /* raw */
        /*int i = 0;
        for (Pair<String, Long> e : PFPGrowth.readFList(context.getConfiguration())) {
            fMap.put(e.getFirst(), i++);
        }*/

        /* version I */
        /*int fListSize = HadoopUtil.readInt(new Path(params.get(PFPGrowth.OUTPUT), PFPGrowth.F_LIST_SIZE), context
                .getConfiguration());
        final float minLoadFactor = 0.8f;
        final float maxLoadFactor = 0.9f;
        final int initCapacity = (int) (fListSize / maxLoadFactor) + 100;
        fMap = new OpenObjectIntHashMap<Integer>(initCapacity, minLoadFactor, maxLoadFactor);
        int i = 0;
        for (Pair<Integer, Integer> e : PFPGrowth.readFList2(context.getConfiguration())) {
            fMap.put(e.getFirst(), i++);
        }*/

        /* version II */
        int fListSize = HadoopUtil.readInt(new Path(params.get(PFPGrowth.OUTPUT), PFPGrowth.F_LIST_SIZE), context
                .getConfiguration());
        fMap = new FixedSizeArrayHashMap(fListSize);
        int i = 0;
        for (Pair<Integer, Integer> e : PFPGrowth.readFList2(context.getConfiguration())) {
            fMap.put(e.getFirst(), i++);
        }
        System.out.println("ParallelFPGrowthMapper.fMap: " + fMap.toSummaryString());

        splitter = Pattern.compile(params.get(PFPGrowth.SPLIT_PATTERN, PFPGrowth.SPLITTER.toString()));
        maxPerGroup = Integer.valueOf(params.getInt(PFPGrowth.MAX_PER_GROUP, 0));

        beginTime = System.currentTimeMillis();
        taskTimeout = Long.parseLong(params.get(PFPGrowth.MAP_TASK_TIMEOUT));
    }
}

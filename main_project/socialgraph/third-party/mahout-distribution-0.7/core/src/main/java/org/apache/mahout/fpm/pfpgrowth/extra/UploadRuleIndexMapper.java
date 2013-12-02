package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import xce.tripod.TripodException;

public class UploadRuleIndexMapper extends Mapper<LongWritable, Text, NullWritable, NullWritable> {

    public enum Counters {
        NUM_SUCC_RECORD, 
        NUM_FAIL_RECORD,
        NUM_INT_ITEM
    };

    @Override
    public void map(LongWritable key, Text val, Context context) throws IOException, InterruptedException {
        String userID = val.toString().split("\t")[0];
        List<Long> ruleIDs = parseList(val.toString().split("\t")[1]);

        try {
            if (CacheUtils.setRuleIndex(userID, ruleIDs)) {
                context.getCounter(Counters.NUM_SUCC_RECORD).increment(1);
                context.getCounter(Counters.NUM_INT_ITEM).increment(ruleIDs.size()*2);
            } else {
                context.getCounter(Counters.NUM_FAIL_RECORD).increment(1);
            }
        } catch (TripodException e) {
            e.printStackTrace();
        }
    }

    /**
     * Parse a long list from string.
     * 
     * @param raw
     * @return
     */
    public static List<Long> parseList(String raw) {
        String data = raw.substring(1, raw.length() - 1);
        List<Long> result = new ArrayList<Long>();

        if (data.length() > 0) {
            for (String item : data.split(", ")) {
                result.add(Long.parseLong(item));
            }
        }
        return result;
    }

    public static void main(String[] args) {
        List<Long> ruleIDs = Arrays.asList();
        System.out.println(parseList(ruleIDs.toString()));

    }
}

package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;
import java.util.List;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import xce.tripod.TripodException;

public class UploadAssociationRulesMapper extends Mapper<LongWritable, Text, NullWritable, NullWritable> {

    public enum Counters {
        NUM_SUCC_RECORD, 
        NUM_FAIL_RECORD,
    };

    @Override
    public void map(LongWritable key, Text val, Context context) throws IOException, InterruptedException {
        int userID = Integer.parseInt(val.toString().split("\t")[0]);
        List<AssociationRule> rules = AssociationRule.parseRuleList(val.toString().split("\t")[1]);

        try {
            if (CacheUtils.setAssociationRuleList(userID, rules)) {
                context.getCounter(Counters.NUM_SUCC_RECORD).increment(1);
            } else {
                context.getCounter(Counters.NUM_FAIL_RECORD).increment(1);
            }
        } catch (TripodException e) {
            e.printStackTrace();
        }
    }
}

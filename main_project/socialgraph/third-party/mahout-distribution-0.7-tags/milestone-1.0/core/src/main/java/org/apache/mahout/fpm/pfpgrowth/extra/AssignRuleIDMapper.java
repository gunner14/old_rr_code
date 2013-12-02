package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.mahout.common.HadoopUtil;
import org.apache.mahout.common.Parameters;
import org.apache.mahout.fpm.pfpgrowth.PFPGrowth;

public class AssignRuleIDMapper extends Mapper<LongWritable, Text, NullWritable, AssociationRule> {

    private static final NullWritable NULL = NullWritable.get();
    private long minRuleID;
    private long maxRuleID;

    @Override
    public void map(LongWritable key, Text val, Context context) throws IOException, InterruptedException {
        String ruleStr = val.toString();
        AssociationRule rule = AssociationRule.parseRule(ruleStr);
        rule.setIndex(getRuleID());
        context.write(NULL, rule);
    }

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
        super.setup(context);
        Parameters params = new Parameters(context.getConfiguration().get(PFPGrowth.PFP_PARAMETERS, ""));
        int numMapTasks = Integer.parseInt(context.getConfiguration().get("mapred.map.tasks"));
        long numAssociationRules = HadoopUtil.readLong(new Path(params.get(PFPGrowth.OUTPUT),
                PFPGrowth.NUM_ASSOCIATION_RULES), context.getConfiguration());
        long maxGroupSize = 2 * (numAssociationRules / numMapTasks + 1);
        int mapTaskID = context.getTaskAttemptID().getTaskID().getId();
        minRuleID = mapTaskID * maxGroupSize;
        maxRuleID = minRuleID + maxGroupSize - 1;
    }

    private long getRuleID() {
        if (minRuleID == maxRuleID) {
            throw new RuntimeException("Association Rule ID Assignment Failed. Caused by: no more ruleID to assign.");
        }
        return minRuleID++;
    }
}

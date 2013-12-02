package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class GeneAssociationRuleReducer extends Reducer<Text, AssociationRule, NullWritable, AssociationRule> {

    public enum Counters {
        NUM_ASSOCIATION_RULES, 
        NUM_INT_ITEM
    };

    @Override
    public void reduce(Text key, Iterable<AssociationRule> vals, Context context) throws IOException,
            InterruptedException {
        // just simply ignore duplicated rules
        AssociationRule rule = vals.iterator().next();
        context.write(NullWritable.get(), rule);

        /* for stat purpose only */
        context.getCounter(Counters.NUM_ASSOCIATION_RULES).increment(1);
        context.getCounter(Counters.NUM_INT_ITEM).increment(rule.getIntSize());
    }

}

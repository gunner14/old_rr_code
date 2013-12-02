package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class GeneAssociationRuleCombiner extends Reducer<Text, AssociationRule, Text, AssociationRule> {

    @Override
    public void reduce(Text key, Iterable<AssociationRule> vals, Context context) throws IOException,
            InterruptedException {
        // just simply ignore duplicated rules
        AssociationRule rule = vals.iterator().next();
        context.write(key, rule);
    }
}

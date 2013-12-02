package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;
import java.util.List;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.mahout.common.Parameters;
import org.apache.mahout.fpm.pfpgrowth.PFPGrowth;
import org.apache.mahout.fpm.pfpgrowth.convertors.string.TopKStringPatterns;

public class GeneAssociationRuleMapper extends Mapper<LongWritable, Text, Text, AssociationRule> {

    private static final Text outKey = new Text();
    private int minConfidence = 90;
    private int maxRulesPerUserGeneFromFP = 1000;

    @Override
    public void map(LongWritable key, Text val, Context context) throws IOException, InterruptedException {
        String mainKey = val.toString().split("\t")[0];
        String ruleStr = val.toString().split("\t")[1];
        TopKStringPatterns patterns = TopKStringPatterns.fromString(ruleStr);
        List<AssociationRule> rules = geneAssociationRules(patterns, mainKey);
        for (AssociationRule rule : rules) {
            outKey.set(rule.toSimpleString());
            context.write(outKey, rule);
        }
    }

    /**
     * Generate association rules, the premise contains mainKey
     * 
     * @param patterns
     * @param mainKey
     * @return
     */
    private List<AssociationRule> geneAssociationRules(TopKStringPatterns patterns, String mainKey) {
        AssociationRuleProducer ruleProducer = new BruteForceAssociationRuleProducer(mainKey, maxRulesPerUserGeneFromFP);
        return ruleProducer.getAssociationRules(patterns, minConfidence);
    }

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
        super.setup(context);
        Parameters params = new Parameters(context.getConfiguration().get(PFPGrowth.PFP_PARAMETERS, ""));
        minConfidence = Integer.parseInt(params.get(PFPGrowth.MIN_CONFIDENCE));
        maxRulesPerUserGeneFromFP = Integer.parseInt(params.get(PFPGrowth.MAX_RULES_PER_USER_GENE_FROM_FP));
    }
}

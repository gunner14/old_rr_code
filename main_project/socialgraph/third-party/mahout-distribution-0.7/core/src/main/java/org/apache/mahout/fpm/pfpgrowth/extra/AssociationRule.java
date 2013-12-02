package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import org.apache.hadoop.io.Writable;

public class AssociationRule implements Writable {

    public static final String PRE_POST_SEP = "->";
    public static final String ITEM_SEP = ",";
    private int[] premise; // rule premise
    private int[] consequence; // rule consequence
    private int support;
    private int confidence; // premiseConsequenceSupp / premiseSupp * 100
    private long index = -1;

    public AssociationRule() {
    }

    public AssociationRule(int[] pre, int[] post, int support, int confidence) {
        super();
        setPremise(pre);
        setConsequence(post);
        this.support = support;
        this.confidence = confidence;
    }

    public AssociationRule(Collection<String> pre, Collection<String> post, int support, int confidence) {
        super();

        int[] tmpPre = new int[pre.size()];
        int i = 0;
        for (String item : pre) {
            tmpPre[i++] = Integer.parseInt(item);
        }
        int[] tmpPost = new int[post.size()];
        i = 0;
        for (String item : post) {
            tmpPost[i++] = Integer.parseInt(item);
        }
        setPremise(tmpPre);
        setConsequence(tmpPost);
        this.support = support;
        this.confidence = confidence;
    }

    public AssociationRule(int[] pre, int[] post, int support, int confidence, long idx) {
        this(pre, post, support, confidence);
        this.index = idx;
    }

    public AssociationRule(Collection<String> pre, Collection<String> post, int support, int confidence, long idx) {
        this(pre, post, support, confidence);
        this.index = idx;
    }

    @Override
    public boolean equals(Object other) {
        if (!(other instanceof AssociationRule)) {
            return false;
        }

        AssociationRule that = (AssociationRule) other;
        if (Arrays.equals(this.premise, that.premise) && Arrays.equals(this.consequence, that.consequence)
                && this.support == that.support && this.confidence == that.confidence && this.index == that.index) {
            return true;
        }

        return false;
    }

    @Override
    public int hashCode() {
        int result = 17;
        for (int pre : premise) {
            result = 31 * result + pre;
        }
        for (int cons : consequence) {
            result = 31 * result + cons;
        }
        result = 31 * result + support;
        result = 31 * result + confidence;
        result = 31 * result + (int) (index ^ (index >>> 32));

        return result;
    }

    /**
     * How many ints(4B) this rule occupy.
     * 
     * @return
     */
    public int getIntSize() {
        return premise.length + consequence.length + 2 + 2;
    }

    /**
     * format: (a,b,c->d,e,f supp conf index)
     */
    @Override
    public String toString() {
        StringBuffer sb = new StringBuffer("(");
        sb.append(toSimpleString());
        sb.append(" " + support);
        sb.append(" " + confidence);
        sb.append(" " + index + ")");
        return sb.toString();
    }

    public static List<AssociationRule> parseRuleList(String raw) {
        List<AssociationRule> result = new ArrayList<AssociationRule>();

        if (raw == null || raw.length() <= 2) {
            return result;
        }

        String[] rules = raw.substring(1, raw.length() - 1).split(", ");
        for (String rule : rules) {
            result.add(parseRule(rule));
        }
        return result;
    }

    /**
     * Parse an association rule from string representation
     * 
     * @param raw string representation of the rule (a,b,c->d,e,f supp conf index)
     * @return
     */
    public static AssociationRule parseRule(String raw) {
        String[] items = raw.substring(1, raw.length() - 1).split(" ");
        // get pre and post
        String[] prepost = items[0].split(PRE_POST_SEP);
        String[] preStr = prepost[0].split(ITEM_SEP);
        int[] tmpPre = new int[preStr.length];
        for (int i = 0; i < tmpPre.length; i++) {
            tmpPre[i] = Integer.parseInt(preStr[i]);
        }
        String[] postStr = prepost[1].split(ITEM_SEP);
        int[] tmpPost = new int[postStr.length];
        for (int i = 0; i < tmpPost.length; i++) {
            tmpPost[i] = Integer.parseInt(postStr[i]);
        }
        // get supp
        int supp = new Float(items[1]).intValue();
        // get conf
        int conf = new Float(items[2]).intValue();
        // get index
        long idx = Long.parseLong(items[3]);

        return new AssociationRule(tmpPre, tmpPost, supp, conf, idx);
    }

    public String toSimpleString() {
        StringBuffer sb = new StringBuffer();
        String sep = "";
        for (int i = 0; i < premise.length; i++) {
            sb.append(sep + premise[i]);
            sep = ITEM_SEP;
        }
        sb.append(PRE_POST_SEP);
        sep = "";
        for (int j = 0; j < consequence.length; j++) {
            sb.append(sep + consequence[j]);
            sep = ITEM_SEP;
        }
        return sb.toString();
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        int preLen = in.readInt();
        int[] tmpPre = new int[preLen];
        for (int i = 0; i < preLen; i++) {
            tmpPre[i] = in.readInt();
        }
        this.premise = tmpPre;

        int postLen = in.readInt();
        int[] tmpPost = new int[postLen];
        for (int i = 0; i < postLen; i++) {
            tmpPost[i] = in.readInt();
        }
        this.consequence = tmpPost;

        this.support = in.readInt();
        this.confidence = in.readInt();
        this.index = in.readLong();
    }

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeInt(premise.length);
        for (int i = 0; i < premise.length; i++) {
            out.writeInt(premise[i]);
        }
        out.writeInt(consequence.length);
        for (int i = 0; i < consequence.length; i++) {
            out.writeInt(consequence[i]);
        }
        out.writeInt(support);
        out.writeInt(confidence);
        out.writeLong(index);
    }

    public String getPreString() {
        String result = new String();
        String sep = "";
        for (int i = 0; i < premise.length; i++) {
            result += (sep + premise[i]);
            sep = ITEM_SEP;
        }
        return result;
    }

    public static int getIntSize(List<AssociationRule> rules) {
        int sum = 0;
        for (AssociationRule rule : rules) {
            sum += rule.getIntSize();
        }
        return sum;
    }

    /**
     * Get the weight of the rule with respect to the specific premiseID
     * 
     * @param preID
     * @return
     */
    public int getWeight(int preID) {
        return support;
    }

    public void setPremise(int[] pre) {
        this.premise = pre;
        Arrays.sort(premise);
    }

    public int[] getPremise() {
        return premise;
    }

    public void setConsequence(int[] post) {
        this.consequence = post;
        Arrays.sort(consequence);
    }

    public int[] getConsequence() {
        return consequence;
    }

    public int getSupport() {
        return support;
    }

    public void setSupport(int support) {
        this.support = support;
    }

    public int getConfidence() {
        return confidence;
    }

    public void setConfidence(int confidence) {
        this.confidence = confidence;
    }

    public long getIndex() {
        return index;
    }

    public void setIndex(long index) {
        this.index = index;
    }

}

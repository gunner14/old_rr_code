package org.apache.mahout.fpm.pfpgrowth.extra;

import java.util.ArrayList;
import java.util.List;

import xce.tripod.TripodException;
import xce.tripod.client.EntryType;
import xce.tripod.client.TripodCacheClient;
import xce.tripod.client.TripodCacheClientFactory;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

public class CacheUtils {

    public static final int EXPIRE_TIME = 31 * 24 * 3600;
    public static final String KEY_PREFIX = "sg_common_";
    public static final int INT_BYTES = 4;
    public static final int CHAR_BYTES = 1;
    public static final String ASSOCIATION_RULE_SUFFIX = "_AR";
    public static final String RULE_INVERT_INDEX_SUFFIX = "_II";
    private static final TripodCacheClient tcc = TripodCacheClientFactory.getClient(EntryType.BIZ_SOCIALGRAPH_COMMON);
    private static Stat stat = new Stat();

    static class Stat {

        private long numRecords = 0;
        private long numBeforeBytes = 0;
        private long numAfterBytes = 0;

        /**
         * Clear all the statistical variables
         */
        public void reset() {
            numRecords = 0;
            numBeforeBytes = 0;
            numAfterBytes = 0;
        }

        public float getCompressRatio() {
            return 100.0f * numAfterBytes / numBeforeBytes;
        }

        public String toSummaryString() {
            return String.format("numRecords:%d, numBeforeBytes:%d, numAfterBytes:%d, compressRatio:%.2f%%",
                    numRecords, numBeforeBytes, numAfterBytes, getCompressRatio());
        }

        /**
         * Update the compress ration
         * 
         * @param before
         * @param after
         */
        public void update(int beforeBytes, int afterBytes) {
            numRecords++;
            numBeforeBytes += beforeBytes;
            numAfterBytes += afterBytes;
        }
    }

    public static void resetStat() {
        stat.reset();
    }

    public static String getSummaryStat() {
        return stat.toSummaryString();
    }

    /**
     * Set the <userID, list(Rule)> to cache
     * 
     * @param userID
     * @param rules
     * @return
     * @throws TripodException
     */
    public static boolean setAssociationRuleList(int userID, List<AssociationRule> rules) throws TripodException {
        Items.Builder result = Items.newBuilder();
        for (AssociationRule rule : rules) {
            /*
             * item.id: support
             * item.msg: index
             * item.fields: confidence, premiseLen, premise[], consequence[]
             */
            Item.Builder item = Item.newBuilder();
            item.setId(rule.getSupport());
            item.setMessage(rule.getIndex() + "");
            item.addFields(rule.getConfidence());
            item.addFields(rule.getPremise().length);
            for (int pre : rule.getPremise()) {
                item.addFields(pre);
            }
            for (int cons : rule.getConsequence()) {
                item.addFields(cons);
            }
            result.addItems(item);
        }

        byte[] resultData = result.build().toByteArray();
        if (null != resultData && resultData.length != 0) {
            // calculate the key/value size
            int keySize = (KEY_PREFIX + userID + ASSOCIATION_RULE_SUFFIX).length() * CHAR_BYTES;
            int valSize = AssociationRule.getIntSize(rules) * INT_BYTES;
            stat.update(keySize + valSize, keySize + resultData.length);
            return tcc.binarySet(userID + ASSOCIATION_RULE_SUFFIX, resultData, EXPIRE_TIME);
        }
        return false;
    }

    public static List<AssociationRule> getAssociationRuleList(int userID) throws TripodException,
            InvalidProtocolBufferException {
        List<AssociationRule> result = new ArrayList<AssociationRule>();
        byte[] rawData = tcc.binaryGet(userID + ASSOCIATION_RULE_SUFFIX);
        Items ruleItems = Items.parseFrom(rawData);
        if (ruleItems != null && ruleItems.getItemsCount() > 0) {
            for (Item ruleItem : ruleItems.getItemsList()) {
                result.add(parseAssociationRule(ruleItem));
            }
        }

        return result;
    }

    private static AssociationRule parseAssociationRule(Item ruleItem) {
        AssociationRule rule = new AssociationRule();
        rule.setSupport(ruleItem.getId());
        rule.setIndex(Long.parseLong(ruleItem.getMessage()));
        List<Integer> data = ruleItem.getFieldsList();
        rule.setConfidence(data.get(0));
        int premiseLen = data.get(1);
        int[] premise = new int[premiseLen];
        int[] consequence = new int[data.size() - 2 - premiseLen];
        for (int i = 0; i < premise.length; i++) {
            premise[i] = data.get(2 + i);
        }
        for (int i = 0; i < consequence.length; i++) {
            consequence[i] = data.get(2 + premiseLen + i);
        }
        rule.setPremise(premise);
        rule.setConsequence(consequence);

        return rule;
    }

    public static boolean remove(String key) {
        boolean success = true;
        try {
            tcc.remove(key);
            stat.update(0, 0);
        } catch (TripodException e) {
            success = false;
            e.printStackTrace();
        }

        return success;
    }

    public static boolean remove(int userID) {
        return remove(userID + ASSOCIATION_RULE_SUFFIX);
    }

    public static boolean setAssociationRule(String ruleID, AssociationRule rule) throws TripodException {
        Items.Builder result = Items.newBuilder();
        // premiseItem contains support and premise[]
        Item.Builder premiseItem = Item.newBuilder();
        premiseItem.setId(rule.getSupport());
        premiseItem.setMessage("F");
        for (Integer apId : rule.getPremise()) {
            premiseItem.addFields(apId);
        }
        result.addItems(premiseItem);

        // consequenceItem contains confidence and consequence[]
        Item.Builder consequenceItem = Item.newBuilder();
        consequenceItem.setId(rule.getConfidence());
        consequenceItem.setMessage("B");
        for (Integer apId : rule.getConsequence()) {
            consequenceItem.addFields(apId);
        }
        result.addItems(consequenceItem);

        byte[] resultData = result.build().toByteArray();

        boolean insert = false;

        if (null != resultData && resultData.length != 0) {
            insert = tcc.binarySet(ruleID + ASSOCIATION_RULE_SUFFIX, resultData, EXPIRE_TIME);
        } else {
            System.out.println("[warn] rule: " + ruleID + " skip!!!!");
        }

        return insert;
    }

    public static AssociationRule getAssociationRule(String ruleId) throws TripodException,
            InvalidProtocolBufferException {
        byte[] rawRule = tcc.binaryGet(ruleId + ASSOCIATION_RULE_SUFFIX);
        Items ruleItems = null;
        ruleItems = Items.parseFrom(rawRule);

        AssociationRule rule = new AssociationRule();;
        if (ruleItems.getItemsCount() != 2) {
            return rule;
        }
        // items[0]
        rule.setSupport(ruleItems.getItems(0).getId());
        List<Integer> fields = ruleItems.getItems(0).getFieldsList();
        int[] premise = new int[fields.size()];
        for (int i = 0; i != fields.size(); i++) {
            premise[i] = fields.get(i);
        }
        rule.setPremise(premise);
        // items[1]
        rule.setConfidence(ruleItems.getItems(1).getId());
        fields = ruleItems.getItems(1).getFieldsList();
        int[] consequence = new int[fields.size()];
        for (int i = 0; i != fields.size(); i++) {
            consequence[i] = fields.get(i);
        }
        rule.setConsequence(consequence);
        return rule;

    }

    public static boolean setRuleIndex(String userID, List<Long> ruleIDs) throws TripodException {
        Items.Builder result = Items.newBuilder();

        Item.Builder item = Item.newBuilder();
        item.setId(Integer.parseInt(userID));
        for (Long apId : ruleIDs) {
            item.addMessages(apId + "");
        }
        result.addItems(item);
        byte[] resultData = result.build().toByteArray();

        boolean insert = false;
        if (null != resultData && resultData.length != 0) {
            insert = tcc.binarySet(userID + RULE_INVERT_INDEX_SUFFIX, resultData, EXPIRE_TIME);
        } else {
            System.out.println("[warn] inverted index of user: " + userID + " skip!!!!");
        }

        return insert;
    }

    public static List<Long> getRuleIndex(String userID) throws TripodException, InvalidProtocolBufferException {

        List<Long> ruleIndexs = new ArrayList<Long>();
        byte[] rawRuleIndex = tcc.binaryGet(userID + RULE_INVERT_INDEX_SUFFIX);
        if (Items.parseFrom(rawRuleIndex).getItemsCount() != 1) {
            return ruleIndexs;
        }

        Item ruleIndexItem = Items.parseFrom(rawRuleIndex).getItems(0);
        for (String index : ruleIndexItem.getMessagesList()) {
            ruleIndexs.add(Long.parseLong(index));
        }
        return ruleIndexs;
    }
    
    public static void main(String[] args) throws InvalidProtocolBufferException, TripodException {
        //System.out.println(getAssociationRuleList(427863118));
        System.out.println(getAssociationRuleList(237145140));
    }

}

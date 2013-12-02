package com.renren.ad.sns.services.antispam.model;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import weka.core.DenseInstance;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;

public class AntiSpamRecord {

    public Date date;

    public long goodsId;

    public long ownerid;

    public float hostReputation;

    public float avgGuestReputation;

    public int totalLikeCnt;

    public float maxIpLikeRatio;

    public int ipCnt;

    public float avgLikeInterval;

    public int spamLevel = SPAM_LEVEL_UNKNOWN;

    /* 真正用于分类的字段个数（包括分类属性），新增/删除字段时，需更改该值 */
    public static final int numFields = 7;

    /* 属性的总数目,不包括分类属性 */
    public static final int numTotalFields = 9;

    private static final SimpleDateFormat dateFormatter = new SimpleDateFormat("yyyy-MM-dd-HH");

    private static Instances dataset;

    private static String wekaModelFile = "model.arff";

    public static final int SPAM_LEVEL_UNKNOWN = -1;

    static {
        try {
            dataset = new DataSource(wekaModelFile).getDataSet();
            dataset.setClassIndex(dataset.numAttributes() - 1);
            //            System.out.println(dataset.toSummaryString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Instance toInstance() {
        Instance inst = new DenseInstance(numFields);
        inst.setDataset(dataset);
        inst.setValue(0, hostReputation);
        inst.setValue(1, avgGuestReputation);
        inst.setValue(2, totalLikeCnt);
        inst.setValue(3, maxIpLikeRatio);
        inst.setValue(4, ipCnt);
        inst.setValue(5, avgLikeInterval);
        if (spamLevel == SPAM_LEVEL_UNKNOWN) {
            inst.setMissing(6);
        } else {
            inst.setValue(6, spamLevel);
        }

        return inst;
    }

    public Instance toInstance(Instances aDataset) {
        Instance inst = new DenseInstance(numFields);
        inst.setDataset(aDataset);
        inst.setValue(0, hostReputation);
        inst.setValue(1, avgGuestReputation);
        inst.setValue(2, totalLikeCnt);
        inst.setValue(3, maxIpLikeRatio);
        inst.setValue(4, ipCnt);
        inst.setValue(5, avgLikeInterval);
        if (spamLevel == SPAM_LEVEL_UNKNOWN) {
            inst.setMissing(6);
        } else {
            inst.setValue(6, spamLevel);
        }

        return inst;
    }

    public static AntiSpamRecord parseLine(String line) {
        String[] items = line.split("\\s+");
        AntiSpamRecord record = null;

        if (items.length < numTotalFields) {
            System.err.println("Bad record:" + line);
        } else {
            record = new AntiSpamRecord();
            try {
                record.date = dateFormatter.parse(items[0]);
                record.goodsId = Long.parseLong(items[1]);
                record.ownerid = Long.parseLong(items[2]);
                record.hostReputation = Float.parseFloat(items[3]);
                record.avgGuestReputation = Float.parseFloat(items[4]);
                record.totalLikeCnt = Integer.parseInt(items[5]);
                record.maxIpLikeRatio = Float.parseFloat(items[6]);
                record.ipCnt = Integer.parseInt(items[7]);
                record.avgLikeInterval = Float.parseFloat(items[8]);
                if (items.length == numTotalFields + 1) {
                    record.spamLevel = Integer.parseInt(items[9]);
                }
            } catch (ParseException e) {
                e.printStackTrace();
            }
        }

        return record;
    }
}

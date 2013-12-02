package com.renren.ad.sns.services.antispam.model;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import weka.classifiers.Classifier;
import weka.core.Instance;


/**
 * 采用分类模型的方法检测Spam Goods, 将结果存入文件中
 * 
 * @author xiangzhang
 * 
 */
public class AntiSpam {

    private Classifier spamClassifier;

    private String spamClassifierFile = "classifier.weka.classifiers.trees.REPTree";

    private List<AntiSpamRecord> spamRecords = new ArrayList<AntiSpamRecord>();

    private String recordsFile;

    private String resultFile = "model_spam_goods";

    private static final Log LOG = LogFactory.getLog(AntiSpam.class);

    //    private static final SimpleDateFormat SDF = new SimpleDateFormat("yyyy-MM-dd-HH");

    public AntiSpam(String recordsFile, String spamClassifierFile) {
        this.spamClassifierFile = spamClassifierFile;
        this.recordsFile = recordsFile;
        this.spamClassifier = ClassifierUtils.loadClassifier(spamClassifierFile);
    }

    public AntiSpam(String recordsFile, String spamClassifierFile, String resultFile) {
        this(recordsFile, spamClassifierFile);
        this.resultFile = resultFile;
    }

    /**
     * 通过分类器，识别可疑的商品，并将可疑商品保存到文件
     */
    public void detectAndSaveToFile() {
        //1. load records, detect spam
        spamRecords.clear();
        List<AntiSpamRecord> records = RecordLoader.loadRecordsFromFile(recordsFile);
        for (AntiSpamRecord record : records) {
            Instance inst = record.toInstance();
            try {
                //0-normal, 1/2/3 spam
                record.spamLevel = (int) spamClassifier.classifyInstance(inst);
                if (record.spamLevel > 0) {
                    spamRecords.add(record);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        LOG.info("Detect spam goods: " + spamRecords.size());

        //2. save result to file
        if (spamRecords.size() > 0) {
            saveSpamToFile(resultFile);
        } else {
            touchFile(resultFile); //创建一个空的结果文件
            LOG.info("No spam found.");
        }
    }

    /**
     * 创建一个新的空文件
     * 
     * @param resultFile
     */
    public static void touchFile(String resultFile) {
        File file = new File(resultFile);
        try {
            file.createNewFile();
        } catch (IOException e) {
            LOG.error(e, e);
        } 
    }

    /**
     * 将spam goods保存成结果文件，每一行的格式：goodsId spamLevel
     * 
     * @param resultFile
     */
    private void saveSpamToFile(String resultFile) {
        List<String> lines = new ArrayList<String>();
        for (AntiSpamRecord asr : spamRecords) {
            lines.add(asr.goodsId + "\t" + asr.spamLevel);
        }

        try {
            FileUtils.writeLines(new File(resultFile), lines);
            LOG.info("write spam goods to file:" + resultFile);
        } catch (IOException e) {
            LOG.error("write result file error: " + e.getLocalizedMessage());
            e.printStackTrace();
        }
    }

    /**
     * 处理检测到的可疑商品
     */
    private void handleSpamGoods() {
        if (spamRecords != null && spamRecords.size() > 0) {
            SpamHandler.handleSpamGoods(spamRecords);
        }
    }

    public static void main(String[] args) {
        if (args.length < 2 || args.length > 3) {
            System.out.println("Usage: " + AntiSpam.class.getSimpleName()
                    + " recordsFile spamClassifierFile [resultFile]");
        } else {
            AntiSpam antiSpam = null;
            if (args.length == 2) {
                antiSpam = new AntiSpam(args[0], args[1]);
            } else {
                antiSpam = new AntiSpam(args[0], args[1], args[2]);
            }

            antiSpam.detectAndSaveToFile();
        }
    }

}

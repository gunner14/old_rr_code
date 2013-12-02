package com.renren.ad.sns.services.antispam.model;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.List;
import java.util.Random;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import weka.classifiers.Classifier;
import weka.classifiers.Evaluation;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;

public class ModelBuilder {

    private static final Log LOG = LogFactory.getLog(ModelBuilder.class);

    private Classifier classifier;

    private Instances trainInstances;

    private Instances evalInstances;

    private String classifierFile = "test-data/classifier";

    private String modelFile = "model.arff";

    private String spamDataFile = "test-data/train_spam";

    private String normDataFile = "test-data/train_normal";

    private String evalDataFile = "test-data/eval_data";

    /**
     * 依次从三个文件中加载以下信息： 1）模型的头文件 2）spam相关的数据 3）normal相关的数据
     * 
     * @param ratio spam/non-spam样本的比例
     * @return
     */
    private Instances loadTrainInstancesFromFile(float ratio) {
        Instances instances = null;

        try {
            // 1. read model header
            /*
             * Reader modelHeader = new InputStreamReader(new
             * FileInputStream(modelFile)); instances = new
             * Instances(modelHeader);
             */
            instances = new DataSource(modelFile).getDataSet();
            instances.setClassIndex(instances.numAttributes() - 1);
            LOG.info("Load model header ok.");

            // 2. read spam instances
            List<String> lines = FileUtils.readLines(new File(spamDataFile));
            int numValidSpamRecord = 0;
            for (String line : lines) {
                AntiSpamRecord record = AntiSpamRecord.parseLine(line);
                if (record != null) {
                    instances.add(record.toInstance(instances));
                    numValidSpamRecord++;
                }
            }
            LOG.info("Load spam records: " + numValidSpamRecord);

            // 3. read normal instances
            lines = FileUtils.readLines(new File(normDataFile));
            int numValidNormRecord = 0;
            for (String line : lines) {
                AntiSpamRecord record = AntiSpamRecord.parseLine(line);
                if (record != null) {
                    instances.add(record.toInstance(instances));
                    numValidNormRecord++;

                    if (ratio != -1 && ((int) (numValidNormRecord * ratio)) >= numValidSpamRecord) {
                        break;
                    }
                }
            }
            LOG.info("Load normal records: " + numValidNormRecord);

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return instances;
    }

    /**
     * 
     * @return
     */
    private Instances loadTestInstancesFromFIle() {
        Instances instances = null;

        try {
            // 1. read model header
            /*
             * Reader modelHeader = new InputStreamReader(new
             * FileInputStream(modelFile)); instances = new
             * Instances(modelHeader);
             */
            instances = new DataSource(modelFile).getDataSet();
            instances.setClassIndex(instances.numAttributes() - 1);
            LOG.info("Load model header ok.");

            // 2. read eval instances
            List<String> lines = FileUtils.readLines(new File(evalDataFile));
            int numValidRecord = 0;
            for (String line : lines) {
                AntiSpamRecord record = AntiSpamRecord.parseLine(line);
                if (record != null) {
                    instances.add(record.toInstance(instances));
                    numValidRecord++;
                }
            }
            LOG.info("Load eval records: " + numValidRecord);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return instances;
    }

    public ModelBuilder(Classifier classifier) {
        super();
        this.classifier = classifier;
    }

    public ModelBuilder(Classifier classifier, String classifierFile) {
        this(classifier);
        this.classifierFile = classifierFile;
    }

    /**
     * 训练分类器模型
     * 
     * @param ratio spam/non-spam样本的比例
     * @return
     */
    public boolean trainModel(float ratio) {
        boolean success = true;

        try {
            // 1. load instances
            trainInstances = loadTrainInstancesFromFile(ratio);

            // 2. train classifier model
            if (trainInstances != null) {
                classifier.buildClassifier(trainInstances);
                LOG.info("Train " + classifier.getClass().getSimpleName() + " model ok.");
            }
        } catch (Exception e) {
            success = false;
            e.printStackTrace();
        }
        return success;
    }

    /**
     * 训练分类器模型，并将模型保存到文件
     * 
     * @return
     */
    public boolean trainAndSaveModel() {
        return trainAndSaveModel(-1);
    }

    public boolean trainAndSaveModel(float ratio) {
        return trainModel(ratio) && ClassifierUtils.saveClassifier(classifier, classifierFile);
    }

    /**
     * 使用交叉验证评测模型的性能指标
     */
    public void crossValidation() {
        try {
            // 1. load instances
            trainInstances = loadTrainInstancesFromFile(-1);

            // 2. evaluate model
            if (trainInstances != null) {
                Evaluation eval = new Evaluation(trainInstances);
                eval.crossValidateModel(classifier, trainInstances, 10, new Random());
                LOG.info(classifier.getClass().getSimpleName() + "\n" + eval.toSummaryString()
                        + "\n" + eval.toMatrixString());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 由于spam/non-spam的数据比例不同，可能存在数据倾斜问题，造成数据验证精度不准。
     * 该方法从文件加载待验证的数据，正负样本的比例为1：1
     */
    public void validation() {
        try {
            final float[] ratios = { 1.0f, 0.5f, 0.333f, 0.25f, 0.2f, 0.1f };

            for (float ratio : ratios) {
                trainModel(ratio);

                evalInstances = loadTestInstancesFromFIle();
                if (classifier != null && evalInstances != null) {
                    Evaluation eval = new Evaluation(trainInstances);
                    eval.evaluateModel(classifier, evalInstances);

                    StringBuffer sb = new StringBuffer();
                    sb.append(classifier.getClass().getSimpleName() + ", ratio:" + ratio + "\n");
                    sb.append(eval.toSummaryString() + "\n");
                    sb.append(eval.toMatrixString());
                    sb.append(String.format("%-10s%-10s%-10s%-10s%-10s\n", "classIdx", "precision",
                            "recall", "Fmeasure", "auc"));
                    for (int i = 0; i < 4; i++) {
                        sb.append(String.format("%-10d%-10.3f%-10.3f%-10.3f%-10.3f\n", i,
                                eval.precision(i), eval.recall(i), eval.fMeasure(i),
                                eval.areaUnderROC(i)));
                    }
                    sb.append(String.format("%-10s%-10.3f%-10.3f%-10.3f%-10.3f\n", "-",
                            eval.weightedPrecision(), eval.weightedRecall(),
                            eval.weightedFMeasure(), eval.weightedAreaUnderROC()));
                    LOG.info(sb.toString());
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    
    
    public void validation(float ratio){
        trainModel(ratio);
        evalInstances = loadTestInstancesFromFIle();
        try{
            if (classifier != null && evalInstances != null) {
                Evaluation eval = new Evaluation(trainInstances);
                eval.evaluateModel(classifier, evalInstances);

                StringBuffer sb = new StringBuffer();
                sb.append(classifier.getClass().getSimpleName() + ", ratio:" + ratio + "\n");
                sb.append(eval.toSummaryString() + "\n");
                sb.append(eval.toMatrixString());
                sb.append(String.format("%-10s%-10s%-10s%-10s%-10s\n", "classIdx", "precision",
                        "recall", "Fmeasure", "auc"));
                for (int i = 0; i < 4; i++) {
                    sb.append(String.format("%-10d%-10.3f%-10.3f%-10.3f%-10.3f\n", i,
                            eval.precision(i), eval.recall(i), eval.fMeasure(i),
                            eval.areaUnderROC(i)));
                }
                sb.append(String.format("%-10s%-10.3f%-10.3f%-10.3f%-10.3f\n", "-",
                        eval.weightedPrecision(), eval.weightedRecall(),
                        eval.weightedFMeasure(), eval.weightedAreaUnderROC()));
                LOG.info(sb.toString());
            }
        }catch (Exception e) {
            e.printStackTrace();
        }
        
    }

    public void setClassifierFile(String classifierFile) {
        this.classifierFile = classifierFile;
    }

    public Instances getInstances() {
        return trainInstances;
    }

    public void setInstances(Instances instances) {
        this.trainInstances = instances;
    }

    public Classifier getClassifier() {
        return classifier;
    }

    public String getClassifierFile() {
        return classifierFile;
    }

    public void testAll() {
        String[] classifiers = new String[] { "weka.classifiers.trees.RandomForest",
                "weka.classifiers.trees.J48", "weka.classifiers.functions.Logistic",
                "weka.classifiers.bayes.NaiveBayes", "weka.classifiers.meta.AdaBoostM1",
                "weka.classifiers.trees.REPTree",
        /* "weka.classifiers.functions.MultilayerPerceptron" */};
        for (String classifier : classifiers) {
            ModelBuilder modelBuilder;
            try {
                modelBuilder = new ModelBuilder((Classifier) Class.forName(classifier)
                        .newInstance());
                modelBuilder.setClassifierFile(modelBuilder.getClassifierFile() + "." + classifier);
                modelBuilder.validation();
            } catch (InstantiationException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }
        }

    }

    public static void main(String[] args) {
        String classifier = "weka.classifiers.trees.REPTree";
        String classifierFile = null;
        if(args.length > 0){
            classifierFile = args[0];
        }else{
            classifierFile = "test-data/classifier." + classifier;
        }
        
        try {
            ModelBuilder modelBuilder = new ModelBuilder((Classifier) Class.forName(classifier)
                    .newInstance());
            modelBuilder.setClassifierFile(classifierFile);
            modelBuilder.trainAndSaveModel(0.5f);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

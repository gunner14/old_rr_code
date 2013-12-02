package com.renren.ad.sns.services.antispam.model;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import weka.classifiers.Classifier;

public class ClassifierUtils {

    private static final Log LOG = LogFactory.getLog(ClassifierUtils.class);

    /**
     * 从模型文件中加载分类器
     * 
     * @param classifierFile
     */
    public static Classifier loadClassifier(String classifierFile) {
        Classifier classifier = null;

        try {
            FileInputStream fis = new FileInputStream(classifierFile);
            classifier = (Classifier) new ObjectInputStream(fis).readObject();
            LOG.info("Load classifier " + classifier.getClass().getSimpleName() + " from "
                    + classifierFile + " ok.");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        return classifier;
    }

    /**
     * 将分类器模型保存到文件
     * 
     * @param classifier
     * @param classifierFile
     */
    public static boolean saveClassifier(Classifier classifier, String classifierFile) {
        boolean success = true;

        if (classifier != null) {
            try {
                ObjectOutputStream oos = new ObjectOutputStream(
                        new FileOutputStream(classifierFile));
                oos.writeObject(classifier);
                LOG.info("Save classifier " + classifier.getClass().getSimpleName() + " to "
                        + classifierFile + " ok.");
            } catch (IOException e) {
                success = false;
                e.printStackTrace();
            }
        } else {
            success = false;
        }
        
        return success;
    }
}

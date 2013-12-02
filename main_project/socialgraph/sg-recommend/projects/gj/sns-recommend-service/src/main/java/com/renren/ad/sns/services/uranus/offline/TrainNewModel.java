package com.renren.ad.sns.services.uranus.offline;

import java.io.*;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;

import weka.classifiers.Classifier;
import weka.classifiers.Evaluation;
import weka.classifiers.bayes.*;
import weka.classifiers.meta.*;
import weka.classifiers.trees.*;
import weka.classifiers.functions.*;

import weka.core.DenseInstance;
import weka.core.Instance;
import weka.core.Instances;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public class TrainNewModel {
	
	private static Logger LOG = Logger.getLogger(TrainNewModel.class);
	private static Instances trainheader;
	
	private static int ClassIndex = 41;
	private static int modelType = 8;

	private static String headerpath = "train.header";
	private static String datapath = "coped";
	private static String modepath = "uranus.model";

	static final int[] OfflineIndex = Util.OfflineIndex;

	static public void main(String args[]) {
		try {
			PropertyConfigurator.configure("log.properties");
			if (args.length > 0) {
				datapath = args[0];
			}
			if (args.length > 1) {
				modepath = args[1];
			}
			if (args.length > 2) {
				modelType = Integer.parseInt(args[2]);
			}

			Reader HeadReader = new InputStreamReader(new FileInputStream(headerpath));
			trainheader = new Instances(HeadReader);
			trainheader.setClassIndex(ClassIndex);
			Instances instancesTrain = new Instances(trainheader);

			BufferedReader reader = new BufferedReader(new InputStreamReader(
					new FileInputStream(datapath)));
			String aline = null;
			HashMap<String, Integer> map = new HashMap<String, Integer>();

			while ((aline = reader.readLine()) != null) {
				try {
					String terms[] = aline.split("\t");
					if(terms.length!=56)
						continue;
					// uid + gid
					String key = terms[1] + terms[2];
					int val = Integer.parseInt(terms[54]); //action
					if (!map.containsKey(key))
						map.put(key, val);
					else if (map.get(key) < val) {
						map.put(key, val);
					}
				} catch (Exception e) {
					LOG.error(e,e);
				}
			}
			reader.close();

			LOG.info("map.size = " + map.size());

			int datanum = map.size();
			int trainnum = (int) (datanum * 0.9);
			trainnum = Math.min(trainnum, 1000000);
			
			LOG.info("load data");
			HashSet<String> set = new HashSet<String>();
			reader = new BufferedReader(new InputStreamReader(
					new FileInputStream(datapath)));
			int j;
			while ((aline = reader.readLine()) != null) {
				try {
					String terms[] = aline.split("\t");
					if(terms.length!=56)
						continue;
					String key = terms[1] + terms[2];
					if (!set.contains(key)) {
						StringBuilder sb = new StringBuilder();
						for( j = 0 ; j < 54 ; j ++ ){
							sb.append(terms[j]+"\t");
						}
						String templine = sb.toString()	+ map.get(key);
						terms = templine.split("\t");
						if (terms.length != 55){
							continue;
						}
						Instance instance = new DenseInstance(ClassIndex + 1);
						instance.setDataset(instancesTrain);
						for (j = 0; j < 35; j++){
							instance.setValue(j,
									Double.parseDouble(terms[OfflineIndex[j]]));
						}
						for (j = 35; j < 42; j++){
							instance.setValue(j, terms[OfflineIndex[j]]);
						}
						instancesTrain.add(instance);
						if (instancesTrain.numInstances() > trainnum){
							break;
						}
						set.add(key);
					}
					if (set.size() > 100000){
						set.clear();
					}
				} catch (Exception e) {
					LOG.error(e,e);
				}
			}
			instancesTrain.setClassIndex(ClassIndex);
			
			LOG.info("train...");
			Classifier m_classifier;
			switch (modelType) {
			case 1:
				m_classifier = new J48();
				LOG.info("Model:J48");
				break;
			case 2:
				m_classifier = new NaiveBayes();
				LOG.info("Model:NaiveBayes");
				break;
			case 3:
				m_classifier = new NaiveBayesMultinomial();
				LOG.info("Model:NaiveBayesMultinomial");
				break;
			case 4:
				m_classifier = new MultilayerPerceptron();
				LOG.info("Model:MultilayerPerceptron");
				break;
			case 5:
				String[] arg = { "-W", "weka.classifiers.trees.J48" };
				AdaBoostM1 c = new AdaBoostM1();
				c.setOptions(arg);
				m_classifier = c;
				LOG.info("Model:AdaBoostM1");
				break;
			case 6:
				m_classifier = new J48();
				LOG.info("Model:J48");
				break;
			case 7:
				m_classifier = new REPTree();
				LOG.info("Model:REPTree");
				break;
			case 8:
				m_classifier = new Logistic();
				LOG.info("Model:Logistic");
				break;
			default:
				m_classifier = new J48();
				LOG.info("Model:J48");
				break;
			}

			Date time1 = new Date();
			m_classifier.buildClassifier(instancesTrain);
			Date time2 = new Date();
			long time = time2.getTime() - time1.getTime();
			LOG.info("training costs " + time + "ms, with "
					+ instancesTrain.numInstances() + " instances");

			FileOutputStream fos = new FileOutputStream(modepath);
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(m_classifier);

			Instances evaInstances = new Instances(trainheader);

			while ((aline = reader.readLine()) != null) {
				try {
					String terms[] = aline.split("\t");
					if(terms.length!=56)
						continue;
					String key = terms[1] + terms[2];
					if (!set.contains(key)) {
						StringBuilder sb = new StringBuilder();
						for( j = 0 ; j < 54 ; j ++ ){
							sb.append(terms[j]+"\t");
						}
						String templine = sb.toString()	+ map.get(key);
						terms = templine.split("\t");
						if (terms.length < 55){
							continue;
						}

						Instance instance = new DenseInstance(ClassIndex + 1);
						instance.setDataset(evaInstances);
						for (j = 0; j < 35; j++){
							instance.setValue(j,
									Double.parseDouble(terms[OfflineIndex[j]]));
						}
						for (j = 35; j < 42; j++){
							instance.setValue(j, terms[OfflineIndex[j]]);
						}
						evaInstances.add(instance);
						if(evaInstances.numInstances() > 200000) break;
						set.add(key);
					}
					if (set.size() > 100000){
						set.clear();
					}
				} catch (Exception e) {
					LOG.error(e,e);
				}
			}
			evaInstances.setClassIndex(ClassIndex);
			Evaluation eva = new Evaluation(evaInstances);

			time1 = new Date();
			eva.evaluateModel(m_classifier, evaInstances);
			time2 = new Date();
			time = time2.getTime() - time1.getTime();

			LOG.info("Evaluation cost " + time + "ms, with "
					+ evaInstances.numInstances() + " Instances, avg:"
					+ ((double) time / evaInstances.numInstances()));
			LOG.info("correct = " + eva.correct());
			LOG.info("errorRate = " + eva.errorRate());
			LOG.info("Avg Precision = " + (1 - eva.errorRate()));
			LOG.info("meanAbsoluteError = " + eva.meanAbsoluteError());
			LOG.info("meanPriorAbsoluteError = " + eva.meanPriorAbsoluteError());
			LOG.info("incorrect = " + eva.incorrect());

			for (j = 0; j < 4; j++) {
				LOG.info("class " + (j + 1) + " : " + "fMeasure = "
						+ eva.fMeasure(j));
				LOG.info("         : " + "precision = "
						+ eva.precision(j));
				LOG.info("         : " + "recall = "
						+ eva.recall(j));

				LOG.info("         : " + "areaUnderROC = "
						+ eva.areaUnderROC(j));
			}

			double m[][] = eva.confusionMatrix();
			int i;
			StringBuilder sb = new StringBuilder();
			sb.append("confusionMatris:\r\n");
			for (i = 0; i < m.length; i++) {
				sb.append("\t");
				for (j = 0; j < m[i].length - 1; j++)
					sb.append(m[i][j] + ",\t");
				sb.append(m[i][m[i].length - 1] + ";\r\n");
			}
			LOG.info(sb.toString());

		} catch (Exception e) {
			LOG.error(e.getMessage());
		}
	}
}
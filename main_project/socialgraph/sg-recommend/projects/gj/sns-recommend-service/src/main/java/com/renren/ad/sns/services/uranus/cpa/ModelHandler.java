package com.renren.ad.sns.services.uranus.cpa;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.Reader;
import java.util.ArrayList;
import java.util.HashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import weka.classifiers.Classifier;
//import weka.classifiers.trees.J48;
import weka.classifiers.functions.Logistic;
import weka.core.DenseInstance;
import weka.core.Instance;
import weka.core.Instances;

import com.renren.ad.sns.services.uranus.util.Util;
import com.renren.ad.sns.util.FileWatchdog;

public class ModelHandler {
	
	private static final Log LOG = LogFactory.getLog(ModelHandler.class);
	private Classifier classifier[] = new Classifier[2];
	private Instances trainHeader;
	private Instances testHeader;
	private String modelPath = "uranus.model";
	private int modelType = Util.MODEL_TYPE_Logistic;
	private int activeClassifier = 0;
	private FileWatchdog modelWatcher;

	private int classIndex = 41;

	private HashMap<Integer, Double> mapper;

	public void init() {
		try {
			Reader reader = new InputStreamReader(new FileInputStream(modelPath
					+ File.separator + "train.header"));
			trainHeader = new Instances(reader);
			reader = new InputStreamReader(new FileInputStream(modelPath
					+ File.separator + "test.header"));
			testHeader = new Instances(reader);
			trainHeader.setClassIndex(classIndex);
			testHeader.setClassIndex(classIndex);
			
			loadModel();
			modelWatcher = new FileWatchdog(this.modelPath + File.separator
					+ "uranus.model") {
				@Override
				protected void doOnChange() {
					try {
						updateModel();
					} catch (Exception e) {
						LOG.error("update model failed!", e);
					}
				}
			};
			modelWatcher.start();

		} catch (Exception e) {
			LOG.error("init ModelHandler Error!", e);
			System.exit(1);
		}
	}

	public void loadModel() throws Exception {
		LOG.info("Load Model");
		FileInputStream fis = new FileInputStream(modelPath + File.separator
				+ "uranus.model");
		ObjectInputStream ois = new ObjectInputStream(fis);

		activeClassifier = 0;

		classifier[0] = Util.createModel(modelType, ois);

		LOG.debug("classifier:" + ((Logistic) classifier[0]).toString());
	}

	public void updateModel() throws Exception {

		LOG.info("Update Model");
		FileInputStream fis = new FileInputStream(modelPath + File.separator
				+ "uranus.model");
		ObjectInputStream ois = new ObjectInputStream(fis);

		try {
			classifier[1 - activeClassifier] = Util.createModel(modelType, ois);
			activeClassifier = 1 - activeClassifier;
		} catch (Exception e) {
			LOG.error("Updating Model Error:", e);
		}
	}

	public ArrayList<Double> predict(ArrayList<String> Records) {
		Instances testInstances = new Instances(testHeader);
		ArrayList<Double> Results = new ArrayList<Double>();
		int i = 0, j = 0;
		for (i = 0; i < Records.size(); i++) {
			String terms[] = Records.get(i).split(Util.SEPERATOR_TERM);
			if (terms == null) {
				LOG.warn("Terms are not valid. terms is NULL");
				Results.add(0.0);
				continue;
			} else if (terms.length < 48) {
				LOG.warn("Terms are not valid. terms.length=" + terms.length);
				Results.add(0.0);
				continue;
			}

			try {
				Instance instance = new DenseInstance(classIndex + 1);
				instance.setDataset(testInstances);
				LOG.debug("Terms:" + Records.get(i));

				for (j = 0; j < 35; j++) {
					instance.setValue(j,
							Double.parseDouble(terms[Util.OnlineIndex[j]]));
					LOG.debug("Attr[" + j + "].name=" + instance.attribute(j)
							+ " Attr[" + j + "].value="
							+ Double.parseDouble(terms[Util.OnlineIndex[j]]));
				}

				for (j = 35; j < 41; j++) {
					instance.setValue(j, terms[Util.OnlineIndex[j]]);
				}

				double value = classifier[activeClassifier]
						.classifyInstance(instance);
				double prob = classifier[activeClassifier]
						.distributionForInstance(instance)[(int)value];
				instance.setClassValue(value);
				Results.add(mapper.get((int) value) * prob);
				testInstances.add(instance);

				
			} catch (Exception e) {
				LOG.error("Error in Predict " + "i=" + i + " j=" + j + " terms[" + Util.OnlineIndex[j] + "]=" + terms[Util.OnlineIndex[j]] + " : "  + Records.get(i),e);
				Results.add(0.0);
			}
		}

		return Results;
	}

	public void setModelPath(String modelPath) {
		this.modelPath = modelPath;
	}

	public void setClassIndex(int classIndex) {
		this.classIndex = classIndex;
	}
	
	public void setMapper(HashMap<Integer,Double> mapper)
	{
		this.mapper = mapper;
	}

}


package com.renren.ad.sns.services.uranus.util;

import java.io.IOException;
import java.io.ObjectInputStream;

import weka.classifiers.Classifier;
import weka.classifiers.bayes.NaiveBayes;
import weka.classifiers.bayes.NaiveBayesMultinomial;
import weka.classifiers.functions.MultilayerPerceptron;
import weka.classifiers.functions.Logistic;
import weka.classifiers.meta.AdaBoostM1;
import weka.classifiers.trees.J48;
import weka.classifiers.trees.REPTree;

public class Util {

	public final static String SEPERATOR_TERM = ",";

	public final static int MODEL_TYPE_DEFAULT = 0;
	public final static int MODEL_TYPE_J48 = 1;
	public final static int MODEL_TYPE_NaiveBayes = 2;
	public final static int MODEL_TYPE_NaiveBayesMultionmial = 3;
	public final static int MODEL_TYPE_MultilayerPerceptron = 4;
	public final static int MODEL_TYPE_AdaBoost = 5;
	public final static int MODEL_TYPE_REPTree = 6;
	public final static int MODEL_TYPE_Logistic = 7;

	public static Classifier createModel(int modelType, ObjectInputStream ois)
			throws ClassNotFoundException, IOException {
		Classifier classifier = null;
		//Model Serialization from File 
		if (ois != null) {
			switch (modelType) {
			case Util.MODEL_TYPE_J48:
				classifier = (J48) ois.readObject();
				break;
			case Util.MODEL_TYPE_NaiveBayes:
				classifier = (NaiveBayes) ois.readObject();
				break;
			case Util.MODEL_TYPE_NaiveBayesMultionmial:
				classifier = (NaiveBayesMultinomial) ois.readObject();
				break;
			case Util.MODEL_TYPE_MultilayerPerceptron:
				classifier = (MultilayerPerceptron) ois.readObject();
				break;
			case Util.MODEL_TYPE_AdaBoost:
				classifier = (AdaBoostM1) ois.readObject();
				break;
			case Util.MODEL_TYPE_REPTree:
				classifier = (REPTree) ois.readObject();
				break;
			case Util.MODEL_TYPE_Logistic:
				classifier = (Logistic) ois.readObject();
				break;
			default:
				classifier = (J48) ois.readObject();
				break;
			}
		}else{
			// Create New Empty Model
			switch (modelType) {
			case Util.MODEL_TYPE_J48:
				classifier =  new J48();
				break;
			case Util.MODEL_TYPE_NaiveBayes:
				classifier = new NaiveBayes();
				break;
			case Util.MODEL_TYPE_NaiveBayesMultionmial:
				classifier = new NaiveBayesMultinomial();
				break;
			case Util.MODEL_TYPE_MultilayerPerceptron:
				classifier = new MultilayerPerceptron();
				break;
			case Util.MODEL_TYPE_AdaBoost:
				classifier = new AdaBoostM1();
				break;
			case Util.MODEL_TYPE_REPTree:
				classifier = new REPTree();
				break;
			case Util.MODEL_TYPE_Logistic:
				classifier = new Logistic();
				break;
			default:
				classifier = new J48();
				break;
			}			
		}
		return classifier;
		
	}
	
	private final static int VISIT_DAY_ONLINE = 9;
	private final static int VISIT_WEEK_ONLINE = 10;
	private final static int CLICK_DAY_ONLINE = 15;
	private final static int CLICK_WEEK_ONLINE = 16;
	private final static int COMMENT_DAY_ONLINE = 13;
	private final static int COMMENT_WEEK_ONLINE = 14;
	private final static int LIKE_DAY_ONLINE = 11;
	private final static int LIKE_WEEK_ONLINE = 12;
	private final static int LOGINTIMES_DAY_ONLINE = 7;
	private final static int LOGINTIMES_WEEK_ONLINE = 8;
	private final static int PUBLISH_DAY_ONLINE = 17;
	private final static int PUBLISH_WEEK_ONLINE = 18;
	private final static int FANSCOUNT_ONLINE = 6;
	private final static int FOLLOWCOUNT_ONLINE = 5;
	private final static int PRICE_ONLINE = 21;
	private final static int LIKECOUNT_ONLINE = 24;
	private final static int SHARECOUNT_ONLINE = 22;
	private final static int COMMENTCOUNT_ONLINE = 23;
	private final static int CATEGORY_CLICK_DAY_ONLINE = 43;
	private final static int CATEGORY_CLICK_OTHER_WEEK_ONLINE = 45;
	private final static int CATEGORY_CLICK_WEEK_ONLINE = 44;
	private final static int CATEGORY_LIKE_DAY_ONLINE = 35;
	private final static int CATEGORY_LIKE_OTHER_WEEK_ONLINE = 37;
	private final static int CATEGORY_LIKE_WEEK_ONLINE = 36;
	private final static int CATEGORY_VISIT_DAY_ONLINE = 31;
	private final static int CATEGORY_VISIT_OTHER_WEEK_ONLINE = 33;
	private final static int CATEGORY_VISIT_WEEK_ONLINE = 32;
	private final static int CATEGORY_COMMENT_DAY_ONLINE = 39;
	private final static int CATEGORY_COMMENT_WEEK_ONLINE = 40;
	private final static int CATEGORY_COMMENT_OTHER_WEEK_ONLINE = 41;
	private final static int CATEGORY_PUBLISH_OTHER_WEEK_ONLINE = 48;
	private final static int OWNER_VISIT_ONLINE = 27;
	private final static int OWNER_LIKE_ONLINE = 28;
	private final static int OWNER_CLICK_ONLINE = 30;
	private final static int OWNER_COMMENT_ONLINE = 29;
	private final static int GENDER_ONLINE = 2;
	private final static int STAGE_ONLINE = 4;
	private final static int ISFAN_ONLINE = 26;
	private final static int ISFOLLOW_ONLINE = 25;
	private final static int WEEKDAY_ONLINE = 50;
	private final static int HOUR_ONLINE = 49;

	public static final int[] OnlineIndex = { VISIT_DAY_ONLINE,
			VISIT_WEEK_ONLINE, CLICK_DAY_ONLINE, CLICK_WEEK_ONLINE,
			COMMENT_DAY_ONLINE, COMMENT_WEEK_ONLINE, LIKE_DAY_ONLINE,
			LIKE_WEEK_ONLINE, LOGINTIMES_DAY_ONLINE, LOGINTIMES_WEEK_ONLINE,
			PUBLISH_DAY_ONLINE, PUBLISH_WEEK_ONLINE, FANSCOUNT_ONLINE,
			FOLLOWCOUNT_ONLINE, PRICE_ONLINE, LIKECOUNT_ONLINE,
			SHARECOUNT_ONLINE, COMMENTCOUNT_ONLINE, CATEGORY_CLICK_DAY_ONLINE,
			CATEGORY_CLICK_OTHER_WEEK_ONLINE, CATEGORY_CLICK_WEEK_ONLINE,
			CATEGORY_LIKE_DAY_ONLINE, CATEGORY_LIKE_OTHER_WEEK_ONLINE,
			CATEGORY_LIKE_WEEK_ONLINE, CATEGORY_VISIT_DAY_ONLINE,
			CATEGORY_VISIT_OTHER_WEEK_ONLINE, CATEGORY_VISIT_WEEK_ONLINE,
			CATEGORY_COMMENT_DAY_ONLINE, CATEGORY_COMMENT_WEEK_ONLINE,
			CATEGORY_COMMENT_OTHER_WEEK_ONLINE,
			CATEGORY_PUBLISH_OTHER_WEEK_ONLINE, OWNER_VISIT_ONLINE,
			OWNER_LIKE_ONLINE, OWNER_CLICK_ONLINE, OWNER_COMMENT_ONLINE,
			GENDER_ONLINE, STAGE_ONLINE, ISFAN_ONLINE, ISFOLLOW_ONLINE,
			WEEKDAY_ONLINE, HOUR_ONLINE, };
	
}

package com.renren.ad.sns.services.uranus.cf.type;


public enum ItemSimilarityType {
    EuclideanDistanceSimilarity, 
    EuclideanDistanceSimilarityWithWeight, 
    PearsonCorrelationSimilarity, 
    PearsonCorrelationSimilarityWithWeight,
    LogLikelihoodSimilarity,
    TanimotoCoefficientSimilarity,
}

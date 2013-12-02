package com.renren.ad.sns.services.uranus.cf.type;


public enum UserSimilarityType {
    EuclideanDistanceSimilarity, 
    EuclideanDistanceSimilarityWithWeight,
    PearsonCorrelationSimilarity,
    PearsonCorrelationSimilarityWithWeight,
    SpearmanCorrelationSimilarity,
    LogLikelihoodSimilarity,
    TanimotoCoefficientSimilarity,
}

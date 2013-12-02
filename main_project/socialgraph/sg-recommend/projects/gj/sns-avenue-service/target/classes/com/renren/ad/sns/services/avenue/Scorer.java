package com.renren.ad.sns.services.avenue;

/**
 * generate scorer
 * @author   xiong.hua
 */
public interface Scorer<T> {
    /**
     * give a score to t
     *
     * @param t
     * @return double 
     */
    public double score(T t);
}


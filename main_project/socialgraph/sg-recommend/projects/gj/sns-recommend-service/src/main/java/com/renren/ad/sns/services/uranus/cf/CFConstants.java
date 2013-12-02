package com.renren.ad.sns.services.uranus.cf;



public class CFConstants {
    
    /* recommendation result */
    public static final String ITEM_TANIMOTO = "/home/xiang.zhang/renrengj-cf/cf-recommendation.dat";
    public static final String recommendResultFile = "cf-recommendation.dat";
    
    /* item weight normalization */
    public static final double OLD_MIN_VALUE = 1.0;
    public static final double OLD_MAX_VALUE = 5.0;
    public static final double OLD_VALUE_RANGE = OLD_MAX_VALUE - OLD_MIN_VALUE;
    /*
     * the weight of categoryPicker is in [3.08,3.14]
     * for simplicity, weight the CF a little bigger, in [3.12, 3.14]
     */
    public static final double NEW_MIN_VALUE = 3.12/*3.14*/;
    public static final double NEW_MAX_VALUE = 3.14/*3.16*/;
    public static final double NEW_VALUE_RANGE = NEW_MAX_VALUE - NEW_MIN_VALUE;
    
    /* file watch dog refresh interval(milliseconds) */
    public static final long DEFAULT_REFRESH_INTERVAL = 3600 * 1000;
    
    
    private static String getRecommendationResult(){
        //TODO: return a recommendation result file according to the specified CF type
        return ITEM_TANIMOTO;
    }
}

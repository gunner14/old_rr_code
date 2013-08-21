package com.xiaonei.click.business.analysis;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class AnalysisFactory implements AnaFactory {

    private static AnaFactory _instance = new AnalysisFactory();

    private volatile Analysts<?> ana = null;

    private AnalysisFactory() {
    }

    public static AnaFactory getInstance() {
        return _instance;
    }

    protected static void setInstance(AnaFactory fac) {
        _instance = fac;
    }

    @Override
    public Analysts<?> buildAnalysts() {
        if (ana == null) {
            synchronized (AnalysisFactory.class) {
                if (ana == null) {
                    ana = new ClickAnalysts();
                }
            }
        }
        return ana;
    }

}

package com.xiaonei.click.business.analysis;

import com.xiaonei.click.BaseTestClass;

public class RecordAnalysisFactory extends BaseTestClass implements AnaFactory {

    private static RecordAnalysisFactory _instance = new RecordAnalysisFactory();

    private RecordAnalysis recoder = new RecordAnalysis();

    private AnaFactory old = null;

    private RecordAnalysisFactory() {
    }

    public static RecordAnalysisFactory getInstance() {
        return _instance;
    }

    @Override
    public RecordAnalysis buildAnalysts() {
        return recoder;
    }

    public void init() {
        old = AnalysisFactory.getInstance();
        AnalysisFactory.setInstance(_instance);
    }

    public void restore() {
        AnalysisFactory.setInstance(old);
    }

}

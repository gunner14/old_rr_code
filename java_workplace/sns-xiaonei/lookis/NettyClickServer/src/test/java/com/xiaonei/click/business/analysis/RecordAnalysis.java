package com.xiaonei.click.business.analysis;

import java.util.Map;

import com.xiaonei.click.BaseTestClass;

public class RecordAnalysis extends BaseTestClass implements Analysts<Object> {

    private Object param = null;

    @Override
    public Object analysis(Map<String, String> req) {
        try {
            param = req;
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    public Object getRecord() {
        return param;
    }

}

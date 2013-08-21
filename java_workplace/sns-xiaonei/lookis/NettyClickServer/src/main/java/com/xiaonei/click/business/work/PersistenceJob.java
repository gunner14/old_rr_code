/**
 * 
 */
package com.xiaonei.click.business.work;

import java.util.Map;

import com.xiaonei.click.business.analysis.AnalysisFactory;

/**
 * 做持久化的工作
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class PersistenceJob implements Job<Void> {

    private Map<String, String> param = null;

    private boolean init = false;

    @SuppressWarnings("unchecked")
    @Override
    public void init(Object... param) {
        if (init) throw new RuntimeException("Reinit!");
        if (param == null || param.length == 0) throw new NullPointerException("Param is Null");
        if (!(param[0] instanceof Map)) throw new NullPointerException("Param type error");
        this.param = (Map<String, String>) param[0];
        init = true;
    }

    @Override
    public Void call() throws Exception {
        if (!init) throw new RuntimeException("Call the " + PersistenceJob.class.getName()
                + " after init the job");
        AnalysisFactory.getInstance().buildAnalysts().analysis(param);
        return null;
    }

}

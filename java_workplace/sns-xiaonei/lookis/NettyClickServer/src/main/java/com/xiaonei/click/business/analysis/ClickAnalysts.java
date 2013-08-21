package com.xiaonei.click.business.analysis;

import java.util.Map;

import org.json.JSONObject;

import com.xiaonei.click.business.persistence.PersistenceManager;

/**
 * 第一个业务，全站点击统计
 * 
 * 业务相关的配置先写死，有时间再整理
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class ClickAnalysts implements Analysts<Void> {

    @Override
    public Void analysis(Map<String, String> req) throws Exception {
        if (req == null) return null;
        String p = null;

        if ((p = req.get("J")) != null) {
            JSONObject jo = null;
            jo = new JSONObject(p);

            jo.put("time", System.currentTimeMillis());
            PersistenceManager.getInstance().set(jo.toString());

        }
        return null;
    }
}

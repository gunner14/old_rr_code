package com.renren.sns.wiki.stat.service.fingerprint;

import java.util.ArrayList;
import java.util.List;

/**
 * 去重策略工厂类
 * 
 * @author yi.li@renren-inc.com since Jul 23, 2012
 * 
 */
public class IdentityStrategyFactory {

    /**
     * 获取策略列表
     * 
     * @return 返回策略列表
     */
    public static List<IdentityStrategy> getStrategies() {
        List<IdentityStrategy> strategies = new ArrayList<IdentityStrategy>();
        HashFingerPrintStrategy hashStrategy = new HashFingerPrintStrategy();
        ImgHashFingerPrintStrategy imgStrategy = new ImgHashFingerPrintStrategy();
        MultiRelatedWordsStrategy multiStrategy = new MultiRelatedWordsStrategy();

        strategies.add(hashStrategy);
        strategies.add(imgStrategy);
        strategies.add(multiStrategy);
        return strategies;
    }

}

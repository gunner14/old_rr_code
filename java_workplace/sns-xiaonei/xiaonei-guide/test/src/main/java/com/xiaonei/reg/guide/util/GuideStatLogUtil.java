package com.xiaonei.reg.guide.util;

import org.apache.log4j.Logger;

/**
 * User: ji.wei
 * Date: 12-4-17
 * Time: 下午2:20
 *
 */

public class GuideStatLogUtil {

    private static Logger logger = Logger.getLogger("guideStatLog");

    public static void log(String logStr){
        logger.info(logStr);
    }
}

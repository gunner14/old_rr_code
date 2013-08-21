package com.renren.sns.wiki.stat.service.fingerprint;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 图片指纹词典
 * <p>
 * 用来存储某一个wiki下的图片指纹列表
 * 
 * @author yi.li@renren-inc.com since Jul 26, 2012
 * 
 */
public class ImageFingerPrintDict {

    // 两组指纹比较时指纹最大差异位数，超过最大差异位判断为两个指纹不相似
    private int diffLevel;

    private Map<String, List<Long>> imgFingerPrintDict = new HashMap<String, List<Long>>();

    private Log logger = LogFactory.getLog(this.getClass());

    public ImageFingerPrintDict(int diffLevel) {
        this.diffLevel = diffLevel;
    }

    public Long checkDuplicateAndSet(String scope, long fingerPrint) {
        List<Long> printList = imgFingerPrintDict.get(scope);
        if (null == printList) {
            printList = new ArrayList<Long>();
            printList.add(fingerPrint);

            imgFingerPrintDict.put(scope, printList);
            return null;
        }

        // TODO 这个地方是否有更好的方式可以判断相似度
        for (Long existPrint : printList) {
            int diff = getDiffPrintLevel(fingerPrint, existPrint);
            if (diff < diffLevel) {
                logger.warn("judage to be the same, diff level: " + diffLevel);
                return existPrint;
            }
        }
        
        printList.add(fingerPrint);
        return null;
    }

    /**
     * 比较两个long行数据，不相同的位数的个数
     * <p>
     * 如：0xff和ox1不相同的个数为7
     */
    public int getDiffPrintLevel(long srcPrint, long cmpPrint) {
        long diff = srcPrint ^ cmpPrint;
        return Long.bitCount(diff);
    }

}

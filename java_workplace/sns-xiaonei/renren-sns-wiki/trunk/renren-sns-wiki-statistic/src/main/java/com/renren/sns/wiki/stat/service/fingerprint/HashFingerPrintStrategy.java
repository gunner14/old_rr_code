package com.renren.sns.wiki.stat.service.fingerprint;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

/**
 * 类Hash方式的指纹计算方法
 * 
 * @author yi.li@renren-inc.com since 2012-7-19
 * 
 */
public class HashFingerPrintStrategy implements IdentityStrategy {

    private long cryptTable[] = null;

    private int threshold = 5;

    private int ShingLen = 4;

    private FingerPrintDict<Integer, Long> dict = new FingerPrintDict<Integer, Long>();
    
    private static Map<Integer, String> significantMap = new HashMap<Integer, String>();

    static {
        significantMap.put(502, "title");
        significantMap.put(110, "playerUrl");
        significantMap.put(601, "title");
        significantMap.put(701, "coverImages.desc");
        //        significantMap.put(709, "title");
    }

    public HashFingerPrintStrategy() {
        preCryptTable();
    }

    private void preCryptTable() {
        long seed = 0x00100001L;
        int index1 = 0;
        int index2 = 0;
        long i = 0;
        cryptTable = new long[5 * 0x10000];
        for (index1 = 0; index1 < 0x10000; index1++) {
            for (index2 = index1, i = 0; i < 5; i++, index2 += 0x10000) {
                long temp1, temp2;

                seed = (seed * 125 + 3) % 0x2AAAAB;
                temp1 = (seed & 0xFFFF) << 0x10;

                seed = (seed * 125 + 3) % 0x2AAAAB;
                temp2 = (seed & 0xFFFF);

                cryptTable[index2] = (temp1 | temp2);
            }
        }
    }

    private long hashString(String str, int dwHashType) {
        long seed1 = 0x7FED7FEDL;
        long seed2 = 0xEEEEEEEEL;
        int i = 0;
        int ch;
        for (i = 0; i < str.length(); i++) {
            ch = str.charAt(i) + 0;
            seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
            seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
        }
        return seed1;
    }

    private long getMinValue(long[] lValue) {
        long lMinValue = Long.MAX_VALUE;
        for (long temp : lValue) {
            if (temp < lMinValue) lMinValue = temp;
        }
        return lMinValue;
    }

    private long getFingerPrint(String str) {
        long lFingerPrint = 0;
        int len = str.length();
        if (len <= threshold) {
            long[] lValue = new long[3];
            lValue[0] = hashString(str, 0);
            lValue[1] = hashString(str, 1);
            lValue[2] = hashString(str, 2);
            lFingerPrint = getMinValue(lValue);
        } else {
            int iEnd = len - ShingLen + 1;
            long[] lFp = new long[iEnd];
            for (int i = 0; i < iEnd; i++) {
                String curStr = str.substring(i, i + ShingLen);
                long[] lValue = new long[3];
                lValue[0] = hashString(curStr, 0);
                lValue[1] = hashString(curStr, 1);
                lValue[2] = hashString(curStr, 2);
                lFp[i] = getMinValue(lValue);
            }
            lFingerPrint = getMinValue(lFp);
        }
        return lFingerPrint;
    }

    public long getFinalFingerPrint(String str) {
        str = str.replaceAll("\\pP|\\pS", " ").replaceAll("\\s+", " ");
        String[] strList = str.split(" ");
        long lFinal = 0;
        for (String temp : strList) {
            lFinal += getFingerPrint(temp);
        }
        return lFinal;
    }

    @Override
    public boolean isDuplicate(int wikiId, int feedType, Map<String, Object> feed) {
        String text = FingerPrintUtil.getSignificantString(significantMap, feed);
        if (StringUtils.isEmpty(text) || "...".equals(text)) {
            return false;
        }

        long fingerPrint = getFinalFingerPrint(text);
        boolean result = dict.checkDuplicateAndSet(wikiId, fingerPrint);
        return result;
    }

    @Override
    public String getStrategyName() {
        return "HashLikeStrategy";
    }

    @Override
    public boolean supportFeedType(int feedType) {
        if (feedType == 502 || feedType == 601 || feedType == 110) {
            return true;
        }
        return false;
    }
}

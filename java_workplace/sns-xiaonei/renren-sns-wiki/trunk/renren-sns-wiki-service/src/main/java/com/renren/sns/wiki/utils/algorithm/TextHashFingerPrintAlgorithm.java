package com.renren.sns.wiki.utils.algorithm;

/**
 * 文本Hash指纹算法，由搜索部门提供
 * 
 * @author yi.li@renren-inc.com since Aug 1, 2012
 * 
 */
public class TextHashFingerPrintAlgorithm {

    private long cryptTable[] = null;

    private int threshold = 5;

    private int shingLen = 4;

    private static volatile TextHashFingerPrintAlgorithm instance = null;

    private TextHashFingerPrintAlgorithm() {
        preCryptTable();
    }

    public static TextHashFingerPrintAlgorithm getInstance() {
        if (null == instance) {
            synchronized (TextHashFingerPrintAlgorithm.class) {
                if (null == instance) {
                    instance = new TextHashFingerPrintAlgorithm();
                }
            }
        }

        return instance;
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
            int iEnd = len - shingLen + 1;
            long[] lFp = new long[iEnd];
            for (int i = 0; i < iEnd; i++) {
                String curStr = str.substring(i, i + shingLen);
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
}

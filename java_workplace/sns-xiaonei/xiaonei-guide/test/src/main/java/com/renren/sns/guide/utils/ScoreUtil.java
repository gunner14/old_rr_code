/**
 * $Id: ScoreUtil.java 21986 2010-05-07 07:18:52Z wei.xiang@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.renren.sns.guide.utils;

import java.util.ArrayList;
import java.util.List;

/**
 * 积分对应的等级，还是写一个配置文件吧，写在类里面修改麻烦
 * 
 * @author wei.xiang(wei.xiang@opi-corp.com)
 * @since 2010-3-8
 */
public class ScoreUtil {

    public final static int[] scores = new int[] { 0, 30, 80, 150, 240, 350,
            480, 560, 800, 990, 1830, 3000, 4230, 5520, 7870, 8280, 9750,
            11280, 12870, 14520, 16230, 18000, 19830, 21720, 23670, 25680,
            27750, 29880, 32070, 34320, 36630, 39000, 41430, 43920, 46470,
            49080, 51750, 54480, 57270, 60120, 80000, 100000, 120000, 150000,
            200000 };

    /**
     * 获取积分等级
     * 
     * @param score 当前积分
     * @return 积分等级和下一级别积分的分数
     */
    public static int[] getLevel(int score) {
        if (score >= 0) {
            for (int i = 0; i < scores.length; i++) {
                if (score < scores[i]) {//小于此级别的积分
                    return new int[] { i, scores[i] };
                }
            }
        }
        return new int[] { -1, -1 };
    }

    /**
     * 等级和连续登录天数转换成图标
     * 
     * @param level 等级
     * @param continueDays 连续登录天数
     * @return 图标地址
     */
    public static List<String> level2Icons(final int level, int continueDays) {
        //if(true) {
            return new ArrayList<String>();
        //}
        /*final String tag = continueDays >= 7 ? "_h.png" : continueDays >= 1 ? "_n.png" : "_l.png";
        final int[] df = { 1, 3, 6, 9, 12, 15 };
        //
        final int max = df[df.length - 1];
        final List<String> ret = new ArrayList<String>(3);
        //
        for (int i = 0; i < (level / max); i++) {
            ret.add((max < 10 ? "0" : "") + max + tag);
        }
        final int moreLevel = level % max;
        if (moreLevel >= 1) {
            for (int i=1;i<df.length;i++) {
                if (moreLevel < df[i]) {
                    ret.add((df[i-1] < 10 ? "0" : "") + df[i-1] + tag);
                    break;
                }
            }
        }
        return ret;*/
    }
}


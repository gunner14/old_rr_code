package com.renren.sns.wiki.utils.remotelog;

import net.paoding.rose.web.var.Model;

/**
 * @author yi.li@renren-inc.com since 2012-3-20
 * 
 */
public class RemoteLogUtil {

    // 完成业务后表示业务完成的参数名
    public static final String FINISH_WORK_MARK = "__remotelog_finish_work";

    /**
     *  标记业务已经成功完成。
     *  一般用于需要成功完成业务后才打log的需求，需要在业务成功完成后调用
     */
    public static void markDone(Model model) {
        model.add(FINISH_WORK_MARK, "1");
    }

    /**
     *  判断业务是否成功完成
     */
    public static boolean isDone(Model model) {
        return model.contains(FINISH_WORK_MARK);
    }
}

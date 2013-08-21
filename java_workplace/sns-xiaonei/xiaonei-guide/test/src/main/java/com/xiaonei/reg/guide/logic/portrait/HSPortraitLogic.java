package com.xiaonei.reg.guide.logic.portrait;

/**
 * 高中阶段的逻辑处理类
 *
 * @author dream
 * @date 2008-10-14
 */
public class HSPortraitLogic {

    static class SingletonHolder {
        static HSPortraitLogic instance = new HSPortraitLogic();
    }

    public static HSPortraitLogic getInstance() {
        return SingletonHolder.instance;
    }
}

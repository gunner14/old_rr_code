package com.xiaonei.reg.guide.logic.portrait;

/**
 * 大学阶段的上传头像页
 *
 * @author dream
 * @date 2008-10-14
 */
public class DXPortraitLogic {

    static class SingletonHolder {
        static DXPortraitLogic instance = new DXPortraitLogic();
    }

    public static DXPortraitLogic getInstance() {
        return SingletonHolder.instance;
    }
}

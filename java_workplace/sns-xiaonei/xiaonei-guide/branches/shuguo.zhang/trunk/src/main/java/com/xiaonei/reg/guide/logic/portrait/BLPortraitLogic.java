package com.xiaonei.reg.guide.logic.portrait;


/**
 * 白领阶段的上传头像页的处理类
 *
 * @author dream
 * @date 2008-10-13
 */
public class BLPortraitLogic {

    static class SingletonHolder {
        static BLPortraitLogic instance = new BLPortraitLogic();
    }

    public static BLPortraitLogic getInstance() {
        return SingletonHolder.instance;
    }


}

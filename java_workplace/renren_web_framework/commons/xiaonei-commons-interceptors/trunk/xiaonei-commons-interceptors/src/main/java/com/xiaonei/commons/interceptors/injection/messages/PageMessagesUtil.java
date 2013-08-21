package com.xiaonei.commons.interceptors.injection.messages;

import net.paoding.rose.web.Invocation;

/**
 * 通过这个类，可以直接获取当前请求的 {@link PageMessages}对象。
 * <p>
 * 控制器中，建议使用方法声明的方式来完成。非控制器的类才使用此接口来处理。
 * 
 * @author 王志亮[zhiliang.wang@opi-corp.com]
 * 
 */
public class PageMessagesUtil {

    /**
     * 获取当前请求
     * 
     * @param inv
     * @return
     */
    public static PageMessages getPageMessages(Invocation inv) {

        PageMessages pms = (PageMessages) inv.getModel().get(PageMessages.MODEL_KEY);
        if (pms == null) {
            pms = new PageMessagesImpl(inv);
            inv.getModel().add(PageMessages.MODEL_KEY, pms);
        }
        return pms;
    }
}

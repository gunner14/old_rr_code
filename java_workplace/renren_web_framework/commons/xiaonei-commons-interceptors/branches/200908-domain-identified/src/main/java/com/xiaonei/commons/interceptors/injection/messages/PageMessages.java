package com.xiaonei.commons.interceptors.injection.messages;

import java.util.List;

import org.springframework.validation.BindingResult;

/**
 * {@link PageMessages} 声明在控制器的action方法中，用于设置给页面的提示信息(不包含请求验证错误信息)
 * <p>
 * 请求验证错误信息，请通过声明 {@link BindingResult}设置
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 */
public interface PageMessages {

    public static final String MODEL_KEY = "pageMessages";

    /**
     * 添加一个普通信息
     * 
     * @param code
     * @param defaultMessage
     */
    public void addMessage(String code, String defaultMessage);

    /**
     * 添加一个警告信息
     * 
     * @param code
     * @param defaultMessage
     */
    public void addWarning(String code, String defaultMessage);

    /**
     * 添加一个错误信息
     * 
     * @param name
     * @param defaultMessage
     */
    public void addError(String name, String defaultMessage);

    /**
     * @param pm
     */
    public void add(PageMessage pm);

    /**
     * @return
     */
    public List<PageMessage> getMessages();
}

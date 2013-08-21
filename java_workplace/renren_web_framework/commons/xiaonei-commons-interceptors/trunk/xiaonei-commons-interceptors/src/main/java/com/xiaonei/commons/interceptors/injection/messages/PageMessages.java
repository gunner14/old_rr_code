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
     */
    public PageMessage addMessage(String code);

    /**
     * 添加一个普通信息
     * 
     * @param code
     * @param defaultMessage
     */
    public PageMessage addMessage(String code, String defaultMessage);

    /**
     * 
     * @param code
     */
    public PageMessage addWarning(String code);

    /**
     * 添加一个警告信息
     * 
     * @param code
     * @param defaultMessage
     */
    public PageMessage addWarning(String code, String defaultMessage);

    /**
     * 
     * @param code
     */
    public PageMessage addError(String code);

    /**
     * 添加一个错误信息
     * 
     * @param code
     * @param defaultMessage
     */
    public PageMessage addError(String code, String defaultMessage);

    /**
     * @return
     */
    public List<PageMessage> getMessages();
}

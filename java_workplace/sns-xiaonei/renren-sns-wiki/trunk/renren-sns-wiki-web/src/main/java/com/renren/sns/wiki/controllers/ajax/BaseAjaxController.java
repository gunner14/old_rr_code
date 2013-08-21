package com.renren.sns.wiki.controllers.ajax;

import net.paoding.rose.web.annotation.Ignored;

import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.utils.JSONResultUtil;
import com.xiaonei.commons.interceptors.Ajax;

/**
 * @author yi.li@renren-inc.com since 2012-4-5
 * 
 */
@Ajax
@Ignored
public class BaseAjaxController {

    /**
     * 向客户端返回错误信息码以及错误信息 格式为{code:"", msg:"", data: { xx:"" }}
     * 
     * @param exception 异常信息
     * @return JSON格式的数据
     */
    public String writeJson(WikiException exception) {
        return writeJson(exception.getCode(), exception.getDisplayMsg(), null);
    }

    /**
     * 向客户端返回响应的数据，数据格式为{code:"", msg:"", data: { xx:"" }}
     * 
     * @param bean 数据bean
     * @return JSON格式数据
     */
    public String writeJsonData(Object data) {
        return this.writeJson(0, "", data);
    }

    /**
     * 向客户端返回响应的数据，数据格式为{code:"", msg:"", data: { xx:"" }}
     * 
     * @param code 信息码
     * @param msg 响应提示信息
     * @param bean 响应数据
     * @return JSON格式数据
     */
    public String writeJson(int code, String msg, Object bean) {
        return "@" + JSONResultUtil.buildJSONResult(code, msg, bean);
    }
}

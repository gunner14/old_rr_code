package com.xiaonei.click.business.analysis;

import java.util.Map;

/**
 * 处理具体业务
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public interface Analysts<T> {

    public T analysis(Map<String, String> req) throws Exception;
}

package com.xiaonei.click.business.persistence;

import java.io.Serializable;

/**
 * 
 * 持久化接口
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public interface Persistence {

    public void set(Serializable o);

    public Object get(Object key);

}

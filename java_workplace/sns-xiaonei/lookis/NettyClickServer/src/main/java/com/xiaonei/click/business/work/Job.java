package com.xiaonei.click.business.work;

import java.util.concurrent.Callable;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public interface Job<T> extends Callable<T> {

    public void init(Object... param);
}

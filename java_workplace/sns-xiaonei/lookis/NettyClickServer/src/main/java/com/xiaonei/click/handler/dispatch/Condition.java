/**
 * 
 */
package com.xiaonei.click.handler.dispatch;

/**
 * 组合模式。用来做dispatch的filter
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public interface Condition {

    public Condition getCondition();

    public boolean check(String URL);

}

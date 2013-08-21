/**
 * 
 */
package com.xiaonei.click.handler.dispatch;

/**
 * 叶子
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public abstract class ConditionSub implements Condition {

    @Override
    public abstract boolean check(String URL);

    @Override
    public Condition getCondition() {
        return this;
    }

}

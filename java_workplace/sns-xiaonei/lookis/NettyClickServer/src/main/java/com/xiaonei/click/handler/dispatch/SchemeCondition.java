/**
 * 
 */
package com.xiaonei.click.handler.dispatch;

/**
 * 还无法获取URL，只有URI
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
@Deprecated
public class SchemeCondition extends ConditionSub {

    String scheme = null;

    public SchemeCondition(String scheme) {
        this.scheme = scheme;
    }

    @Override
    public boolean check(String URL) {
        if (URL == null) return scheme == null ? true : false;
        return URL.startsWith(scheme);
    }
}

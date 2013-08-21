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
public class DomainCondition extends ConditionSub {

    String domain = null;

    public DomainCondition(String domain) {
        this.domain = domain;
    }

    @Override
    public boolean check(String URL) {
        if (URL == null) return domain == null ? true : false;
        String[] subUrl = URL.split("/");
        if (subUrl.length < 3) return false;
        return subUrl[2].equalsIgnoreCase(domain);

    }
}

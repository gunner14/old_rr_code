/**
 * 
 */
package com.xiaonei.click.handler.dispatch;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class URICondition extends ConditionSub {

    String uri = null;

    public URICondition(String uri) {
        this.uri = uri;
    }

    @Override
    public boolean check(String URL) {
        if (URL == null) return emptyUri() ? true : false;
        return URL.startsWith(uri);

    }

    private boolean emptyUri() {
        return uri == null || uri.equalsIgnoreCase("/") || uri.equalsIgnoreCase("");
    }
}

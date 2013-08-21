package com.xiaonei.click.handler.http;

import java.util.Map;

import org.jboss.netty.handler.codec.http.HttpRequest;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public interface HttpParser {

    public Map<String, String> parse(HttpRequest req) throws Exception;
}

package com.renren.sns.wiki.utils.cache;


/**
 * Cache的命名空间
 * 
 * @author yi.li@renren-inc.com since 2012-3-22
 *
 */
public enum CacheNameSpace {
    TEST("social_wiki"),
    WIKI("social_wiki");

    private CacheNameSpace(String namespace) {
        this.namespace = namespace;
    }

    private String namespace;

    public String getNamespace() {
        return this.namespace;
    }
}

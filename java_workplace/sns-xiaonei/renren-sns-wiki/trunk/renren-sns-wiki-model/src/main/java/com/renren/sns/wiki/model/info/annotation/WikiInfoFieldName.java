package com.renren.sns.wiki.model.info.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 属性对应的数据库name字段的名字, @WikiInfoFieldName( { "别名", "外文名", "英文名"
 * }),第一个最好别改,用户排重需要
 * 
 * @author weiwei.wang@renren-inc.com since 2012-3-30
 * 
 */
@Target( { ElementType.FIELD })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface WikiInfoFieldName {

    String[] value();

}

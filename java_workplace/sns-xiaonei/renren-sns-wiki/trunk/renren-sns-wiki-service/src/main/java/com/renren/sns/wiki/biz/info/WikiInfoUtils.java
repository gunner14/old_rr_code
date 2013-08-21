package com.renren.sns.wiki.biz.info;

import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.info.Book;
import com.renren.sns.wiki.model.info.Game;
import com.renren.sns.wiki.model.info.Movie;
import com.renren.sns.wiki.model.info.Place;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.model.info.WikiInfoProperty;
import com.renren.sns.wiki.model.info.annotation.WikiInfoFieldName;

/**
 * 
 * List<WikiInfoProperty>和WikiInfo子类的相互转化
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 * 
 */
public class WikiInfoUtils {

    private static final Log logger = LogFactory.getLog(WikiInfoUtils.class);

    private static Map<Class<? extends WikiInfo>, Map<Field, Set<String>>> wikiInfoMap = new HashMap<Class<? extends WikiInfo>, Map<Field, Set<String>>>();

    public static Map<Field, Set<String>> getWikiInfoFiledMap(Class<? extends WikiInfo> c) {
        if (!wikiInfoMap.containsKey(c)) {
            Field[] fields = c.getDeclaredFields();
            Map<Field, Set<String>> wikiInfoFiledNameMap = new HashMap<Field, Set<String>>();
            for (Field field : fields) {
                boolean hasAnnotation = field.isAnnotationPresent(WikiInfoFieldName.class);
                if (hasAnnotation) {
                    WikiInfoFieldName annotation = field.getAnnotation(WikiInfoFieldName.class);
                    String[] originNames = annotation.value();
                    if (originNames != null) {
                        Set<String> set = new LinkedHashSet<String>();
                        for (String originName : originNames) {
                            set.add(originName);
                        }
                        wikiInfoFiledNameMap.put(field, set);
                    }
                }
            }//for field end
            wikiInfoMap.put(c, wikiInfoFiledNameMap);
        }
        return wikiInfoMap.get(c);
    }

    /**
     * 根据wikiType创建对应的WikiInfo的子类
     * 
     * @param wikiType
     * @return
     */
    public static Class<? extends WikiInfo> getWikiInfoSubclass(WikiType wikiType) {
        Class<? extends WikiInfo> c = null;
        switch (wikiType) {
            case BOOK:
                c = Book.class;
                break;
            case MOVIE:
                c = Movie.class;
                break;
            case GAME:
                c = Game.class;
                break;
            case PLACE:
                c = Place.class;
                break;
            default:
                break;
        }
        return c;
    }

    /**
     * 把wikiInfo中propertyList中可以放到wikiInfo的属性赋值到wikiInfo,
     * 并从propertyList移除该属值
     * 当wikiInfo.getPropertyList()为NULL时抛出WikiException异常
     * 
     *IllegalAccessException,IllegalArgumentException由 field.set(wikiInfo,
     * value); 抛出
     * 
     * @param propertyList
     * @param wikiInfo
     * @throws IllegalAccessException
     * @throws IllegalArgumentException
     * @throws WikiException
     */
    public static void parsePropertyList(WikiInfo wikiInfo) throws IllegalArgumentException,
            IllegalAccessException, WikiException {
        if (wikiInfo == null) {
            return;
        }
        List<WikiInfoProperty> propertyList = wikiInfo.getPropertyList();
        if (propertyList == null) {
            throw new WikiException(WikiExceptionType.WIKI_INFO_PROPERTY_LIST_NULL);
        }

        Class<? extends WikiInfo> c = wikiInfo.getClass();

        Map<Field, Set<String>> wikiInfoFiledMap = getWikiInfoFiledMap(c);

        Iterator<WikiInfoProperty> it = propertyList.iterator();
        while (it.hasNext()) {
            WikiInfoProperty wikiInfoProperty = it.next();
            String name = wikiInfoProperty.getName();
            String value = wikiInfoProperty.getValue();

            for (Entry<Field, Set<String>> entry : wikiInfoFiledMap.entrySet()) {
                Field field = entry.getKey();
                Set<String> set = entry.getValue();
                if (set.contains(name)) {
                    field.setAccessible(true);//设置允许访问   
                    field.set(wikiInfo, value);
                    it.remove();
                    break;
                }
            }

        }// while end

    }

    /**
     * 
     * 将wikiInfo带有WikiInfoFieldName注解的属性的值复制到wikiInfo的propertyList属性中,并返回
     * 
     * (String) field.get(wikiInfo)可能抛出IllegalArgumentException,
     * IllegalAccessException异常
     * 
     * @param wikiInfo
     * @return
     * @throws IllegalArgumentException
     * @throws IllegalAccessException
     */
    public static List<WikiInfoProperty> parseWikiInfo(WikiInfo wikiInfo)
            throws IllegalArgumentException, IllegalAccessException {

        List<WikiInfoProperty> propertyList = wikiInfo.getPropertyList();
        Class<? extends WikiInfo> c = wikiInfo.getClass();

        Map<Field, Set<String>> wikiInfoFiledMap = getWikiInfoFiledMap(c);

        //        Field[] fields = c.getDeclaredFields();

        for (Entry<Field, Set<String>> entry : wikiInfoFiledMap.entrySet()) {
            Field field = entry.getKey();
            Set<String> set = entry.getValue();

            if (set.size() > 0) {
                WikiInfoProperty wikiInfoProperty = new WikiInfoProperty();
                wikiInfoProperty.setName(set.iterator().next());
                String value = null;
                field.setAccessible(true);//设置允许访问   
                value = (String) field.get(wikiInfo);
                wikiInfoProperty.setValue(value);
                wikiInfoProperty.setFieldName(field.getName());
                wikiInfoProperty.setWikiId(wikiInfo.getWikiId());

                propertyList.add(wikiInfoProperty);
            }
        }//for field end

        return propertyList;
    }

    //转化name, 比如 简介 简 介  summary Summary返回统一名字
    public static String handlerWikiInfoPropertyName(WikiType wikiType, String name) {
        try {
            Class<? extends WikiInfo> c = WikiInfoUtils.getWikiInfoSubclass(wikiType);
            Map<Field, Set<String>> wikiInfoFiledMap = getWikiInfoFiledMap(c);
            for (Set<String> set : wikiInfoFiledMap.values()) {
                if (set.contains(name)) {
                    return set.iterator().next();
                }
            }
        } catch (Exception e) {
            logger.warn("过滤名字发生异常", e);
            return name;
        }
        return name;
    }
}

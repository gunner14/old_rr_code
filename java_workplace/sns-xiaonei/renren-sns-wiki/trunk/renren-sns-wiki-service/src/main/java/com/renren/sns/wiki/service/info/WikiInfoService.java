package com.renren.sns.wiki.service.info;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.info.WikiInfoBiz;
import com.renren.sns.wiki.biz.info.WikiInfoUtils;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.model.info.WikiInfoProperty;
import com.renren.sns.wiki.service.BaseService;
import com.renren.sns.wiki.service.WikiService;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-3-30
 * 
 */
@Service
public class WikiInfoService extends BaseService {

    @Autowired
    private WikiInfoBiz wikiInfoBiz;

    @Autowired
    private WikiService wikiService;

    /**
     * 获取wiki的资料
     * 
     * @param wiki
     * @return
     */
    public WikiInfo getWikiInfoByWiki(Wiki wiki) {
        if (wiki == null) {
            return null;
        }
        try {
            int wikiId = wiki.getId();
            List<WikiInfoProperty> propertyList = wikiInfoBiz.getPropertyListByWikiId(wikiId);
            WikiType wikiType = WikiType.getEnum(wiki.getType());
            return wikiInfoBiz.createWikiInfo(wiki.getId(), wikiType, propertyList);
        } catch (Exception e) {
            logger.warn("getWikiInfoByWiki failure", e);
        }
        return null;
    }

    /**
     * 添加或者更改Wiki资料,返回更新成功条数
     * 
     * @param propertyList
     */
    public int setWikiInfo(List<WikiInfoProperty> propertyList, WikiType wikiType) {
        int count = 0;
        for (WikiInfoProperty wikiInfoProperty : propertyList) {
            boolean result = wikiInfoBiz.saveOrUpdateWikiInfoProperty(wikiInfoProperty, wikiType);
            if (result) {
                count++;
            }
        }
        return count;
    }
    
    /**
     * 得到wiki资料的json信息
     * 
     * @param wiki
     * @return
     */
    public Map<String, Object> getWikiInfoMap(Wiki wiki) {
        if (wiki == null) {
            logger.warn("getWikiInfoMap(Wiki wiki) param wiki is null");
            throw new WikiException(WikiExceptionType.WIKI_NOT_EXISTS, "wiki对象为空");
        }
        WikiInfo wikiInfo = getWikiInfoByWiki(wiki);

        //不同类型对应不同的信息
        Map<String, Object> subInfoMap = new HashMap<String, Object>();

        WikiType wikiType = WikiType.getEnum(wiki.getType());
        Class<? extends WikiInfo> c = WikiInfoUtils.getWikiInfoSubclass(wikiType);

        Map<Field, Set<String>> fieldMap = WikiInfoUtils.getWikiInfoFiledMap(c);
        for (Field field : fieldMap.keySet()) {
            try {
                field.setAccessible(true);
                String value = (String) field.get(wikiInfo);
                if (value == null) {
                    value = "";
                }
                subInfoMap.put(field.getName(), value);
            } catch (IllegalArgumentException e) {
                logger.warn("field.get(wikiInfo)出错", e);
            } catch (IllegalAccessException e) {
                logger.warn("field.get(wikiInfo)出错", e);
            }
        }
        Map<String, Object> map = wikiService.getWikiMap(wiki);

        map.put("subInfo", subInfoMap);

        return map;
    }

    /**
     * 根据wikiId获取wikiInfoProperty的list列表
     * 
     * @param wikiId
     * @return
     */
    public List<WikiInfoProperty> getPropertyListByWikiId(int wikiId) {
        return wikiInfoBiz.getPropertyListByWikiId(wikiId);
    }

    /**
     * 根据id删除wiki资料
     * 
     * @param wikiInfoPropertyId
     */
    public int deleteWikiInfoPropertyById(int wikiInfoPropertyId) {
        return wikiInfoBiz.deleteWikiInfoPropertyById(wikiInfoPropertyId);
    }

    public boolean saveOrUpdateWikiInfoProperty(WikiInfoProperty wikiInfoProperty, WikiType wikiType) {
        return wikiInfoBiz.saveOrUpdateWikiInfoProperty(wikiInfoProperty, wikiType);
    }

    /**
     * 修改或者添加资料
     * 
     * @param name
     * @param value
     * @return
     */
    public boolean saveOrUpdateWikiInfoProperty(int wikiId, String name, String value,
            WikiType wikiType) {

        if (logger.isDebugEnabled()) {
            logger.debug("saveOrUpdateWikiInfoProperty name=" + name + ", value=" + value
                    + ", wikiType=" + wikiType.getWikiType());
        }

        WikiInfoProperty wikiInfoProperty = new WikiInfoProperty();
        wikiInfoProperty.setWikiId(wikiId);
        wikiInfoProperty.setName(name);
        wikiInfoProperty.setValue(value);
        return wikiInfoBiz.saveOrUpdateWikiInfoProperty(wikiInfoProperty, wikiType);
    }

    /**
     * wikiInfoList全部内容,如果bean中有字段,加上空的value
     * 
     * @param wiki
     * @return
     */
    public List<WikiInfoProperty> getFullWikiInfoService(Wiki wiki) {
        if (wiki == null) {
            return null;
        }
        List<WikiInfoProperty> wikiInfoPropertylist = this.getPropertyListByWikiId(wiki.getId());

        List<WikiInfoProperty> fullWikiInfoPropertylist = new ArrayList<WikiInfoProperty>();

        if (wiki != null) {
            Class<? extends WikiInfo> c = WikiInfoUtils.getWikiInfoSubclass(WikiType.getEnum(wiki
                    .getType()));
            Map<Field, Set<String>> wikiInfoFiledMap = WikiInfoUtils.getWikiInfoFiledMap(c);
            for (Set<String> set : wikiInfoFiledMap.values()) {//bean属性
                boolean has = false;
                if (wikiInfoPropertylist != null) {
                    Iterator<WikiInfoProperty> it = wikiInfoPropertylist.iterator();
                    while (it.hasNext()) {
                        WikiInfoProperty bean = it.next();
                        if (set.contains(bean.getName())) {
                            has = true;
                            fullWikiInfoPropertylist.add(bean);
                            it.remove();
                            break;
                        }
                    }
                }
                if (!has) {
                    WikiInfoProperty bean = new WikiInfoProperty();
                    bean.setName(set.iterator().next());
                    bean.setValue("");
                    fullWikiInfoPropertylist.add(bean);
                }
            }//for end

            //额外属性
            if (wikiInfoPropertylist != null) {
                Iterator<WikiInfoProperty> it = wikiInfoPropertylist.iterator();
                while (it.hasNext()) {
                    WikiInfoProperty bean = it.next();
                    fullWikiInfoPropertylist.add(bean);
                }
            }
        }
        return fullWikiInfoPropertylist;
    }
}

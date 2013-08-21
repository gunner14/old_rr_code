package com.renren.sns.wiki.biz.info;

import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.dao.WikiInfoPropertyDAO;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.model.info.WikiInfoProperty;

@Service
public class WikiInfoBiz extends BaseBiz {

    private static final Log logger = LogFactory.getLog(WikiInfoBiz.class);

    @Autowired
    private WikiInfoPropertyDAO wikiInfoPropertyDAO;

    public List<WikiInfoProperty> getPropertyListByWikiId(int wikiId) {
        return wikiInfoPropertyDAO.getPropertyListByWikiId(wikiId);
    }

    /**
     * 根据wikiId和name获取id
     * 
     * @param wikiId
     * @param name
     * @return
     */
    public Integer getWikiInfoPropertyId(int wikiId, String name) {
        Integer result = wikiInfoPropertyDAO.getWikiInfoPropertyId(wikiId, name);
        if (logger.isDebugEnabled()) {
            logger.debug(String.format(
                    "getWikiInfoPropertyId() where wikiId=%d, name=%s, result=%d", wikiId, name,
                    result));
        }
        return result;
    }

    /**
     * 根据wikiType创建对应的WikiInfo的子类
     * 
     * @param wikiType
     * @return
     */
    public Class<? extends WikiInfo> getWikiInfoSubclass(WikiType wikiType) {
        return WikiInfoUtils.getWikiInfoSubclass(wikiType);
    }

    public WikiInfo createWikiInfo(int wikiId, WikiType wikiType,
            List<WikiInfoProperty> propertyList) {
        try {
            WikiInfo bean = WikiInfoUtils.getWikiInfoSubclass(wikiType).newInstance();

            if (bean == null) {
                return null;
            }
            bean.setPropertyList(propertyList);
            bean.setWikiId(wikiId);
            if(bean.getSummary()==null){
                bean.setSummary("");
            }
            //propertyList可以转移到bean的属性转移到bean中
            WikiInfoUtils.parsePropertyList(bean);
            return bean;
        } catch (Exception e) {
            logger.error("createWikiInfo() error", e);
            throw new RuntimeException(e);
        }
    }

    /**
     * 更改某个属性
     * 
     * @param wikiInfoProperty
     */
    public boolean saveOrUpdateWikiInfoProperty(WikiInfoProperty wikiInfoProperty, WikiType wikiType) {

        if (wikiInfoProperty == null) {
            logger.warn("wikiInfoProperty不能为null");
            return false;
        }
        
        if(wikiInfoProperty.getWikiId() <= 0){
            logger.warn("未设置wikiId");
            return false;
        }

        //查询是否包含某个属性 查询条件wikiId和name 
        String name = wikiInfoProperty.getName();
        String value = wikiInfoProperty.getValue();
        if (StringUtils.isBlank(name) || StringUtils.isBlank(value)) {
            return false;
        }
        if (wikiInfoProperty.getPriority() == 0) {
            //未设置优先级,默认设置优先级为10
            wikiInfoProperty.setPriority(10);
        }

        //将同义词处理成标准的名字,并设置在wikiInfoProperty.name
        String standardName = WikiInfoUtils.handlerWikiInfoPropertyName(wikiType, name);
        wikiInfoProperty.setName(standardName);

        if (logger.isDebugEnabled()) {
            logger.debug("wikiInfoProperty name=" + name + ", standardName=" + standardName);
        }
        Integer wikiInfoPropertyId = getWikiInfoPropertyId(wikiInfoProperty.getWikiId(),
                standardName);
        int updateCount = 0;
        if (wikiInfoPropertyId == null) {
            updateCount = wikiInfoPropertyDAO.addWikiInfoProperty(wikiInfoProperty);
        } else {
            int priority = wikiInfoProperty.getPriority();
            updateCount = wikiInfoPropertyDAO.updateWikiInfoProperty(wikiInfoPropertyId,
                    standardName, value, priority);
        }
        if (updateCount > 0) {
            return true;
        } else {
            return false;
        }
    }

    public int deleteWikiInfoPropertyById(int wikiInfoPropertyId) {
        return wikiInfoPropertyDAO.deleteWikiInfoPropertyByWikiId(wikiInfoPropertyId);
    }

}

package com.renren.sns.wiki.biz.adminwiki;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.adminwiki.OperateIndexDAO;
import com.renren.sns.wiki.model.admin.OperateIndex;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-20
 * 
 */
@Service
public class OperateIndexBiz {

    @Autowired
    private OperateIndexDAO operateIndexDAO;

    public int addOperateIndex(OperateIndex operateIndex) {
        int result = operateIndexDAO.addOperateIndex(operateIndex);
        return result;
    }

    public Integer getWikiId(int wikiId, String operateType) {
        return operateIndexDAO.getWikiId(wikiId, operateType);
    }

    /**
     * 根据wikiType和operateType返回运营列表
     * 
     * @param wikiType
     * @param operateIndexType
     * @return
     */
    public List<OperateIndex> getWikiIdList(int wikiType, String operateType) {
        return operateIndexDAO.getWikiIdList(wikiType, operateType);
    }

    public int delete(int wikiId, String operateType) {
        return operateIndexDAO.delete(wikiId, operateType);
    }

}

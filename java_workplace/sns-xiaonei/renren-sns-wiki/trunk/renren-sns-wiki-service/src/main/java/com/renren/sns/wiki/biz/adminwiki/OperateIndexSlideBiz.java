package com.renren.sns.wiki.biz.adminwiki;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.adminwiki.OperateIndexSlideDAO;
import com.renren.sns.wiki.model.admin.OperateIndexSlide;

/**
 * 运营首页幻灯片
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-21
 * 
 */
@Service
public class OperateIndexSlideBiz {

    @Autowired
    private OperateIndexSlideDAO operateIndexSlideDAO;

    public int addOperateIndexSlide(OperateIndexSlide operateIndexSlide) {
        int result = operateIndexSlideDAO.addOperateIndexSlide(operateIndexSlide);
        return result;
    }

    public Integer getWikiId(int wikiId) {
        return operateIndexSlideDAO.getWikiId(wikiId);
    }

    /**
     * 根据wikiType和operateType返回运营列表
     * 
     * @param wikiType
     * @param operateIndexType
     * @return
     */
    public List<OperateIndexSlide> getWikiIdList(int wikiType) {
        return operateIndexSlideDAO.getWikiIdList(wikiType);
    }

    public int delete(int wikiId) {
        return operateIndexSlideDAO.delete(wikiId);
    }

    public OperateIndexSlide getOperateIndexSlideByWikiId(int wikiId) {
        return operateIndexSlideDAO.getOperateIndexSlideByWikiId(wikiId);
    }

    public OperateIndexSlide getOperateIndexSlideById(int id) {
        return operateIndexSlideDAO.getOperateIndexSlideById(id);
    }

    public int updateOperateIndexSlide(OperateIndexSlide operateIndexSlide) {
        return operateIndexSlideDAO.updateOperateIndexSlide(operateIndexSlide);
    }

    public int deleteOperateIndexSlideByWikiId(int wikiId) {
        return operateIndexSlideDAO.deleteOperateIndexSlideByWikiId(wikiId);
    }

}

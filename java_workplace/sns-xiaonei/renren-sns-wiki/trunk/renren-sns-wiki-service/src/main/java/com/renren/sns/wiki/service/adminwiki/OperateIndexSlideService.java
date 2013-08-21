package com.renren.sns.wiki.service.adminwiki;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.adminwiki.OperateIndexSlideBiz;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.model.admin.OperateIndexSlide;

/**
 * 运营首页slide
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-21
 * 
 */
@Service
public class OperateIndexSlideService {

    @Autowired
    private OperateIndexSlideBiz operateIndexSlideBiz;

    public int addOperateIndexSlide(OperateIndexSlide operateIndexSlide) {
        int result = operateIndexSlideBiz.addOperateIndexSlide(operateIndexSlide);
        return result;
    }

    /**
     * wikiId是否已经存在于运营列表里
     * 
     * @param wikiId
     * @param operateType
     * @return
     */
    public boolean contains(int wikiId) {
        Integer id = operateIndexSlideBiz.getWikiId(wikiId);
        if (id != null) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * 根据wikiType和operateType返回运营列表
     * 
     * @param wikiType
     * @param operateIndexType
     * @return
     */
    public List<OperateIndexSlide> getList(WikiType wikiType) {
        return operateIndexSlideBiz.getWikiIdList(wikiType.getWikiType());
    }

    /**
     * 从运营列表中删除
     * 
     * @param wikiId
     * @param operateIndexType
     * @return
     */
    public int delete(int wikiId) {
        return operateIndexSlideBiz.delete(wikiId);
    }

    public OperateIndexSlide getOperateIndexSlideByWikiId(int wikiId) {
        return operateIndexSlideBiz.getOperateIndexSlideByWikiId(wikiId);
    }
    
    public OperateIndexSlide getOperateIndexSlideById(int id) {
        return operateIndexSlideBiz.getOperateIndexSlideById(id);
    }

    public int updateOperateIndexSlide(OperateIndexSlide operateIndexSlide) {
        return operateIndexSlideBiz.updateOperateIndexSlide(operateIndexSlide);
    }

    public int deleteByWikiId(int wikiId) {
        return operateIndexSlideBiz.deleteOperateIndexSlideByWikiId(wikiId);
    }

}

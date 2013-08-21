package com.renren.sns.wiki.service.adminwiki;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.adminwiki.OperateIndexBiz;
import com.renren.sns.wiki.enums.OperateIndexType;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.model.UserActionRecord;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.admin.OperateIndex;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.service.UserActionRecordService;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.info.WikiInfoService;
import com.xiaonei.platform.core.model.User;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-20
 * 
 */
@Service
public class OperateIndexService {

    @Autowired
    private OperateIndexBiz operateIndexBiz;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private WikiInfoService wikiInfoService;

    @Autowired
    private UserActionRecordService userActionRecordService;

    public int addOperateIndex(OperateIndex operateIndex) {
        int result = operateIndexBiz.addOperateIndex(operateIndex);
        return result;
    }

    /**
     * wikiId是否已经存在于运营列表里
     * 
     * @param wikiId
     * @param operateType
     * @return
     */
    public boolean contains(int wikiId, OperateIndexType operateIndexType) {
        Integer id = operateIndexBiz.getWikiId(wikiId, operateIndexType.getValue());
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
    public List<OperateIndex> getWikiIdList(WikiType wikiType, OperateIndexType operateIndexType) {
        return operateIndexBiz.getWikiIdList(wikiType.getWikiType(), operateIndexType.getValue());
    }

    /**
     * 从运营列表中删除
     * 
     * @param wikiId
     * @param operateIndexType
     * @return
     */
    public int delete(int wikiId, OperateIndexType operateIndexType) {
        return operateIndexBiz.delete(wikiId, operateIndexType.getValue());
    }

    /**
     * 首页wiki列表需要的wiki信息
     * 
     * @param user
     * @param wikiIdList
     * @return
     */
    public List<Map<String, Object>> getIndexWikiMapList(User user, List<Integer> wikiIdList) {

        List<Map<String, Object>> fullInfoMapList = new ArrayList<Map<String, Object>>();
        for (int wikiId : wikiIdList) {
            Wiki wiki = wikiService.getWikiById(wikiId);
            WikiInfo wikiInfo = wikiInfoService.getWikiInfoByWiki(wiki);
            UserActionRecord userActionRecord = null;
            if (user != null) {
                userActionRecord = userActionRecordService
                        .getUserActionRecord(wikiId, user.getId());
            }
            fullInfoMapList.add(getInfoMap(user, wiki, wikiInfo, userActionRecord));
        }

        return fullInfoMapList;
    }

    /**
     * 用于返回首页wiki列表的显示
     * 
     * @param user
     * @param wiki
     * @param wikiInfo
     * @param userActionRecord
     * @return
     */
    public Map<String, Object> getInfoMap(User user, Wiki wiki, WikiInfo wikiInfo,
            UserActionRecord userActionRecord) {
        Map<String, Object> infoMap = new HashMap<String, Object>();
        Map<String, Object> wikiMap = wikiService.getWikiMap(wiki);

        //填充summary
        wikiMap.put("summary", wikiInfo.getSummary());

        infoMap.put("wikiInfo", wikiMap);
        Map<String, Object> interestInfoMap = null;
        if (userActionRecord != null) {
            interestInfoMap = userActionRecordService.getSimpleInterestInfoMap(userActionRecord);
        } else {
            interestInfoMap = new HashMap<String, Object>();
        }
        infoMap.put("userActionRecordInfo", interestInfoMap);

        return infoMap;
    }

}

package com.renren.sns.wiki.controllers.ajax;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.UserActionRecord;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.service.UserActionRecordService;
import com.renren.sns.wiki.service.info.WikiInfoService;
import com.renren.sns.wiki.service.search.WikiSearchService;
import com.renren.sns.wiki.utils.PaginationList;
import com.xiaonei.commons.interceptors.access.HostHolder;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * @author yi.li@renren-inc.com since Aug 15, 2012
 * 
 */
public class SearchController extends BaseAjaxController {

    @Autowired
    private WikiSearchService wikiSearchService;

    @Autowired
    private WikiInfoService wikiInfoService;

    @Autowired
    private UserActionRecordService userActionRecordService;

    @Autowired
    private HostHolder hostHolder;

    @Get("")
    @Post("")
    public String index(@Param("kw") String keyWord) {
        if (StringUtils.isEmpty(keyWord) || StringUtils.isEmpty(keyWord.trim())) {
            return this.writeJson(WikiExceptionType.SEARCH_NULL_KEYWORDS.getCode(),
                    WikiExceptionType.SEARCH_NULL_KEYWORDS.getDisplayMsg(), null);
        }
        String searchKeyWord = keyWord.trim();
        // 限制查询长度
        if (searchKeyWord.length() > 50) {
            searchKeyWord = searchKeyWord.substring(0, 50);
        }

        PaginationList<Wiki> result = wikiSearchService.queryWiki(searchKeyWord);
        Map<String, Object> data = new HashMap<String, Object>();

        List<Map<String, String>> wikiList = buildResult(result.getPageList());
        data.put("wikiList", wikiList);

        return this.writeJsonData(data);
    }

    private List<Map<String, String>> buildResult(List<Wiki> wikis) {
        List<Map<String, String>> wikiList = new ArrayList<Map<String, String>>();
        for (Wiki wiki : wikis) {
            Map<String, String> dataMap = new HashMap<String, String>();
            dataMap.put("wikiId", wiki.getId() + "");
            dataMap.put("name", wiki.getName());
            dataMap.put("tinyUrl", wiki.getHeadUrl());
            dataMap.put("type", wiki.getType() + "");
            // wrap info
            WikiInfo wikiInfo = wikiInfoService.getWikiInfoByWiki(wiki);
            dataMap.put("summary", wikiInfo.getSummary());
            // wrap action
            if (null != hostHolder.getUser()) {
                UserActionRecord uaRecord = userActionRecordService.getUserActionRecord(
                        wiki.getId(), hostHolder.getUser().getId());
                if (null != uaRecord) {
                    dataMap.put("interest", uaRecord.getInterest());
                }
            }

            wikiList.add(dataMap);
        }

        return wikiList;
    }
}

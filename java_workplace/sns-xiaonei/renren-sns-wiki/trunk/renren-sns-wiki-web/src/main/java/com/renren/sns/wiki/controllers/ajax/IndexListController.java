package com.renren.sns.wiki.controllers.ajax;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.enums.OperateIndexType;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.model.admin.OperateIndex;
import com.renren.sns.wiki.service.adminwiki.OperateIndexService;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-17
 * 
 */
@Path("/")
public class IndexListController extends BaseAjaxController {


    @Autowired
    private HostHolder hostHolder;


    @Autowired
    private OperateIndexService operateIndexService;

    @Get("/${wikiType:\\w+}/latest")
    public String latest(Invocation inv, @Param("wikiType") String wikiTypeName) {
        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);
        if (wikiType == null) {
            return "e:404";
        }

        //获取最新的12个列表 select id from wiki 
        List<OperateIndex> operateIndexList = operateIndexService.getWikiIdList(wikiType,
                OperateIndexType.LATEST);
        List<Integer> wikiIdList = new ArrayList<Integer>();
        for (OperateIndex operateIndex : operateIndexList) {
            wikiIdList.add(operateIndex.getWikiId());
        }
        User host = hostHolder.getUser();
        
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("hasMore", false);
        map.put("wikiList", operateIndexService.getIndexWikiMapList(host, wikiIdList));

        return writeJsonData(map);
    }

    @Get("/${wikiType:\\w+}/hot")
    public String hot(Invocation inv, @Param("wikiType") String wikiTypeName) {
        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);
        if (wikiType == null) {
            return "e:404";
        }
      //获取最新的12个列表 select id from wiki 
        List<OperateIndex> operateIndexList = operateIndexService.getWikiIdList(wikiType,
                OperateIndexType.HOT);
        List<Integer> wikiIdList = new ArrayList<Integer>();
        for (OperateIndex operateIndex : operateIndexList) {
            wikiIdList.add(operateIndex.getWikiId());
        }
        User host = hostHolder.getUser();
        
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("hasMore", false);
        map.put("wikiList", operateIndexService.getIndexWikiMapList(host, wikiIdList));

        return writeJsonData(map);
    }

}

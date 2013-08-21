package com.renren.sns.wiki.controllers.ajax;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.enums.InterestType;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.UserActionRecordService;
import com.renren.sns.wiki.service.WikiService;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;

@Path("/movie2")
public class MovieController extends BaseAjaxController {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private UserActionRecordService userActionRecordService;

    @Get("/movie-list")
    public String movieList(Invocation inv) {

        String order = "desc";
        int pageSize = 100;
        int page = 0;
        int endId = 0;
        int curPage = 0;
        int type = WikiType.MOVIE.getWikiType();
        boolean light = true;
        List<Wiki> movieList = wikiService.getWikiList(light, type, curPage, endId, page, pageSize,
                order);

        User host = hostHolder.getUser();
        int code = 0;
        if (host == null) {
            code = WikiExceptionType.LOGIN_REQUIRED.getCode();
        }

        Map<String, Object> map = new HashMap<String, Object>();

        List<Map<String, Object>> fullInfoMapList = new ArrayList<Map<String, Object>>();
        for (Wiki wiki : movieList) {
            Map<String, Object> infoMap = new HashMap<String, Object>();
            Map<String, Object> wikiMap = wikiService.getWikiMap(wiki);
            infoMap.put("wikiInfo", wikiMap);
            if (host != null) {
                Map<String, Object> interestInfoMap = userActionRecordService
                        .getSimpleInterestInfoMap(wiki.getId(), host.getId());
                infoMap.put("userActionRecordInfo", interestInfoMap);
            }
            fullInfoMapList.add(infoMap);
        }

        map.put("hasMore", false);
        map.put("wikiList", fullInfoMapList);

        return writeJson(code, "", map);
    }

    @Get("/movie-list/follow")
    @LoginRequired
    public String movieListFollow(Invocation inv) {
        return "@follow";
    }

    @Get("/movie-list/wish")
    @LoginRequired
    public String movieListWish(Invocation inv) {
        User host = hostHolder.getUser();
        List<Integer> wikiIdList = userActionRecordService.getWikiIdList(host.getId(),
                WikiType.MOVIE, InterestType.WISH);
        return writeJsonData(getMap(host, wikiIdList));
    }

    @Get("/movie-list/collect")
    @LoginRequired
    public String movieListCollect(Invocation inv) {
        User host = hostHolder.getUser();
        List<Integer> wikiIdList = userActionRecordService.getWikiIdList(host.getId(),
                WikiType.MOVIE, InterestType.COLLECT);
        return writeJsonData(getMap(host, wikiIdList));
    }

    @Get("/movie-list/2012")
    public String movieList2012(Invocation inv) {
        User host = hostHolder.getUser();
        List<Integer> wikiIdList = new ArrayList<Integer>();
        for (int wikiId : WikiConstant.movieIds2012) {
            wikiIdList.add(wikiId);
        }
        return writeJsonData(getMap(host, wikiIdList));
    }
    
    @Get("/movie-list/2011")
    public String movieList2011(Invocation inv) {
        User host = hostHolder.getUser();
        List<Integer> wikiIdList = new ArrayList<Integer>();
        for (int wikiId : WikiConstant.movieIds2011) {
            wikiIdList.add(wikiId);
        }
        return writeJsonData(getMap(host, wikiIdList));
    }
    
    @Get("/movie-list/earlier")
    public String movieListEarlier(Invocation inv) {
        User host = hostHolder.getUser();
        List<Integer> wikiIdList = new ArrayList<Integer>();
        for (int wikiId : WikiConstant.movieIdsEarlier) {
            wikiIdList.add(wikiId);
        }
        return writeJsonData(getMap(host, wikiIdList));
    }

    private Map<String, Object> getMap(User user, List<Integer> wikiIdList) {

        Map<String, Object> map = new HashMap<String, Object>();

        List<Map<String, Object>> fullInfoMapList = new ArrayList<Map<String, Object>>();
        for (int wikiId : wikiIdList) {
            Wiki wiki = wikiService.getWikiById(wikiId);
            Map<String, Object> infoMap = new HashMap<String, Object>();
            Map<String, Object> wikiMap = wikiService.getWikiMap(wiki);
            infoMap.put("wikiInfo", wikiMap);
            if (user != null) {
                Map<String, Object> interestInfoMap = userActionRecordService
                        .getSimpleInterestInfoMap(wiki.getId(), user.getId());
                infoMap.put("userActionRecordInfo", interestInfoMap);
            }
            fullInfoMapList.add(infoMap);
        }

        map.put("hasMore", false);
        map.put("wikiList", fullInfoMapList);

        return map;
    }

}

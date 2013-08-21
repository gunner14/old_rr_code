package com.xiaonei.page.xoa.controllers;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.atpage.AtNotifyEnum;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.SettingsService;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 为@Page提供接口服务
 * 1、校验被@的Page是否合法;
 * 2、发送
 * User: shunlong.jin@renren-inc.com
 * Date: 12-3-6
 * Time: 下午5:15
 */
public class AtPageController {
    private final static Logger logger = Logger.getLogger("PAGEXOA");
    @Autowired
    private PageService pageService;

    @Autowired
    private AuthService authService;

    /**
     * 判断这些id是否合法
     *
     * @param ids
     * @return
     */
    @Get("isCorrect")
    public Map isCorrect(@Param("ids") List<Integer> ids) {
        Map<Integer, String> re = new HashMap<Integer, String>();
        try {
            List<Page> pages = pageService.getPages(ids);
            for (Page page : pages) {
                re.put(page.getId(), page.getName());
            }
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
        }
        return re;
    }

    /**
     * 哪些Page应该收到通知
     *
     * @param hostId
     * @param pages
     * @return
     */
    @Get("notifyPages")
    public List<Integer> getNotifyPages(@Param("hostId") int hostId, @Param("pages") List<Integer> pages) {
        List<Integer> re = new ArrayList<Integer>();
        try {
            FriendsHome friendsHome = FriendsHome.getInstance();
            SettingsService settingsService = SettingsService.getInstance();
            for (Integer pageId : pages) {
                Page page = pageService.get(pageId);
                char atSet = settingsService.getSettingsChar(page, SettingsService.BitIndex.AT_PAGE_SETTING);
                if (AtNotifyEnum.ALL.getVal() == atSet) {
                    //全部通知
                    re.add(pageId);
                } else if (AtNotifyEnum.FANS.getVal() == atSet) {
                    //只有粉丝
                    if (authService.isPageActiveFans(hostId, pageId)) {
                        re.add(pageId);
                    }
                } else if (AtNotifyEnum.FRIEND.getVal() == atSet) {
                    //只有好友
                    if (friendsHome.isFriend(pageId, hostId)) {
                        re.add(pageId);
                    }
                } else if (AtNotifyEnum.FRIEND_FANS.getVal() == atSet) {
                    if (authService.isPageActiveFans(hostId, pageId) || friendsHome.isFriend(pageId, hostId)) {
                        re.add(pageId);
                    }
                }
            }
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
        }
        return re;
    }

}

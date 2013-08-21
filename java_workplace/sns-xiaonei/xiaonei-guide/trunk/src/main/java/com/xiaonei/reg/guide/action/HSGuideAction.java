package com.xiaonei.reg.guide.action;

import java.sql.SQLException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.datum.config.BuildSchedule;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.usercount.UserCount;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.pokey.base.logic.LogicFactory;
import com.xiaonei.pokey.base.model.PetUser;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.register.logic.additional.RegInviterInviteeLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.reg.register.model.RegInviteMap;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.log.TimeCost;

/**
 * 新注册的用户引导的最后一步，用户登录少于6次时所要到的页面，高中阶段
 * 
 * @author seraph
 */
public class HSGuideAction extends ProtectAction {

    public ActionForward executePt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws SQLException {
        TimeCost cost = TimeCost.begin("HSGuideAction");
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:" + host.getId() + " NOHERE! ", GuideLogger.ERROR);
        // 防止直接访问将要分发的action.do
        String vid = (String) request.getAttribute("G" + String.valueOf(host.getId()));
        if (vid == null || !String.valueOf(host.getId()).equals(vid)) {
            return null;
        }
        String forward = "ok";
        // 教育部用户注册引导时去的guide页面
        if ("edu".equals(RegFromSourceLogic.getFromSource(host)) && host.getLoginCount() == 1) {
            forward = "mntOfEdu";
        }
        try {
            // 登录者的信息
            long beginUserBasic = LogTimeUtils.logBegin();
            UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
            request.setAttribute("userBasic", ub);
            LogTimeUtils.logEnd(beginUserBasic, "EndUserBasic");
            cost.endStep("1");
            // 得到邀请者的信息
            long beginInviter = LogTimeUtils.logBegin();
            RegInviteMap inviter = RegInviterInviteeLogic.getInstance().getRegInviteMapByInvitee(host.getId());
            int inviterId = 0;
            if (inviter != null) {
                inviterId = inviter.getInviterId();
            }
            LogTimeUtils.logEnd(beginInviter, "EndInviter");
            cost.endStep("2");
            // 邀请者信息
            long beginInviterId = LogTimeUtils.logBegin();
            request.setAttribute("inviter", UserAdapter.getUser(inviterId));
            LogTimeUtils.logEnd(beginInviterId, "EndInviterId");
            cost.endStep("3");
            long beginFirstTime = LogTimeUtils.logBegin();            
            LogTimeUtils.logEnd(beginFirstTime, "EndFirstTime");
            cost.endStep("4");
            request.setAttribute("loginCount", host.getLoginCount());  
            /* 各类申请数 */
            long beginUserCounts = LogTimeUtils.logBegin();
            initUserCounts(request, host);
            LogTimeUtils.logEnd(beginUserCounts, "EndUserCounts");
            cost.endStep("7");
            // nnd不知道是啥应该和页面上的新留言及回复提示有关系
            long beginfangleList = LogTimeUtils.logBegin();           
            LogTimeUtils.logEnd(beginfangleList, "EndfangleList");
            cost.endStep("8");
            // 获得同高中的同学，其实是设置条件,因为是到后面进行一次并发执行
            long beginhsList = LogTimeUtils.logBegin();            
            LogTimeUtils.logEnd(beginhsList, "EndhsList");
            cost.endStep("9");
            // 获得同初中的同学，其实是设置条件，因为是到后面进行一次并发执行
            long beginjuniorName = LogTimeUtils.logBegin();
            LogTimeUtils.logEnd(beginjuniorName, "EndjuniorName");
            cost.endStep("10");
            // 获得同小学的同学，其实是设置条件，因为是到后面进行一次并发执行
            long beginelementaryName = LogTimeUtils.logBegin();
            LogTimeUtils.logEnd(beginelementaryName, "EndelementaryName");
            cost.endStep("11");
            // 执行搜索
            long begineSearchAdapter = LogTimeUtils.logBegin();
            LogTimeUtils.logEnd(begineSearchAdapter, "EndSearchAdapter");
            cost.endStep("12");            
            // 处理新鲜事
            long beginFeedInfo = LogTimeUtils.logBegin(); 
            LogTimeUtils.logEnd(beginFeedInfo, "EndFeedInfo");
            cost.endStep("14");
            long beginreplyList = LogTimeUtils.logBegin();            
            LogTimeUtils.logEnd(beginreplyList, "EndreplyList");
            cost.endStep("15");
        } catch (Exception e) {
            cost.endException(e);
            e.printStackTrace();
        } finally {
            cost.endFinally();
        }
        /*
         * 关闭开心农场公告的cookie读取
         */
        try {
            if (CookieManager.getInstance().getCookie(request, "csl_hs") != null) {
                request.setAttribute("hidedog", "1");
            } else {
                PetUser petUser = LogicFactory.getPetUser().get(host.getId());
                if (petUser != null && petUser.getHas_pet() > 0) {
                    request.setAttribute("hidedog", "1");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        BuildSchedule bu = new BuildSchedule(host);
        request.setAttribute("buildSchedule", bu);
        return mapping.findForward(forward);
    }

    private void initUserCounts(HttpServletRequest request, User host) {
        try {
            List<UserCount> counters = UserCountMgr.getCounts(host.getId());
            if (counters != null) {
                for (UserCount uc : counters) {
                    if (uc != null) {
                        switch (uc.getType()) {
                            case UserCountMgr.RECOMMENDED_FRIEND:
                                request.setAttribute("recommendedCount", uc.getCount());// 好友推荐个数
                                break;
                            case UserCountMgr.POKE:
                                request.setAttribute("pokeCount", uc.getCount());// 招呼个数
                                break;
                            case UserCountMgr.GUEST_REQUEST:
                                request.setAttribute("buddyApplyCount", uc.getCount());// 好友申请个数
                                break;
                            case UserCountMgr.EVENT_INVITE:
                                request.setAttribute("eventInviteCount", uc.getCount());// 活动邀请个数
                                break;
                            case UserCountMgr.REG_APPLY:
                                request.setAttribute("highSchoolApplyCount", uc.getCount());// 高中邀请个数
                                break;
                            case UserCountMgr.PHOTO_TAG_INVITE:
                                request.setAttribute("photoTagRequestCount", uc.getCount());// 圈人请求个数
                                break;
                            case UserCountMgr.SURVEY_INVITE:
                                request.setAttribute("surveyInviteCount", uc.getCount());// 投票邀请个数
                                break;
                            case UserCountMgr.APP_INVITE:
                                request.setAttribute("appinvitecount", uc.getCount());// 应用请求个数
                                break;
                            case UserCountMgr.NOTIFY:
                                request.setAttribute("notifycount", uc.getCount());// 通知个数
                                break;
                            case UserCountMgr.GAME_INVITE:
                                request.setAttribute("gameInviteCount", uc.getCount());// 游戏邀请个数
                                break;
                        }
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 得到热门分享
     * 
     * @param request
     */
    /*
     * @SuppressWarnings("unchecked") private void
     * getHotShares(HttpServletRequest request) { // 取得热门视频 List<Map>
     * hotVideoLst = HSGuideLogic.getInstance().getHotVideo(); if (hotVideoLst
     * != null && hotVideoLst.size() > 0) { request.setAttribute("hotVideo",
     * hotVideoLst.get(0)); } // 取得热门音频 List<Map> hotAudioLst =
     * HSGuideLogic.getInstance().getHotAudio(); if (hotAudioLst != null &&
     * hotAudioLst.size() > 0) { request.setAttribute("hotAudio",
     * hotAudioLst.get(0)); } // 取得热门相册 List<Map> hotPhotoAlbumLst =
     * HSGuideLogic.getInstance().getHotPhotoAlbum();
     * request.setAttribute("hotPhotoAlbumLst", hotPhotoAlbumLst); // 取得热门日志
     * List<Map> hotBlogLst = HSGuideLogic.getInstance().getHotBlog();
     * request.setAttribute("hotBlogLst", hotBlogLst); }
     */

}

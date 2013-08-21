package com.xiaonei.reg.guide.action;

import java.sql.SQLException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.math.NumberUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.datum.config.BuildSchedule;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.usercount.UserCount;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.pokey.base.logic.LogicFactory;
import com.xiaonei.pokey.base.model.PetUser;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.view.UserTinyView;
import com.xiaonei.reg.register.logic.additional.RegInviterInviteeLogic;
import com.xiaonei.reg.register.model.RegInviteMap;
import com.xiaonei.xce.log.TimeCost;

/**
 * 新注册的用户引导的最后一步，用户登录少于6次时所要到的页面，白领阶段、什么都不是阶段
 *
 * @author seraph
 */
public class BLGuideAction extends ProtectAction {
    public ActionForward executePt(ActionMapping mapping, ActionForm form,HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        TimeCost cost = TimeCost.begin("BLGuideAction");
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        // 防止直接访问将要分发的action.do
        String vid = (String) request.getAttribute("G" + String.valueOf(host.getId()));
        if (vid == null || !String.valueOf(host.getId()).equals(vid)) {
            return null;
        }
        String forward = "ok";
        try {
            // 得到邀请者的信息
            RegInviteMap inviter = RegInviterInviteeLogic.getInstance().getRegInviteMapByInvitee(host.getId());
            int inviterId = 0;
            if (inviter != null) {
                inviterId = inviter.getInviterId();
            }
            cost.endStep("1");
            // 邀请者信息
            request.setAttribute("inviter", UserAdapter.getUser(inviterId));            
            // 新增：第一次登录后，guide该显示的业务的处理 Chris Gao 11/28/2008
            request.setAttribute("loginCount", host.getLoginCount());
            cost.endStep("8");
            // 你可能认识的人
            List<UserTinyView> ipFriendsList = null;
            if (inviter != null) {
                try {                   
                } catch (Exception e) {
                    e.printStackTrace();
                }
                request.setAttribute("hasMoreFri", true);
            } else {
            }
            request.setAttribute("ipFriendsList", ipFriendsList);
            cost.endStep("10");            
            /* 各类申请数 */
            initUserCounts(request, host);
            cost.endStep("11");           
            cost.endStep("12");
            cost.endStep("13");
            try {// 大通告部分
                int cookieId = NumberUtils.toInt(CookieManager.getInstance().getCookie(request, "csl_dx"));
                switch (cookieId) {
                    case 0:// 狗狗
                        PetUser petUser = LogicFactory.getPetUser().get(host.getId());
                        if (petUser != null && petUser.getHas_pet() > 0) {
                            request.setAttribute("showdog", 2);
                        } else {
                            // 第一次使用或者未领养狗狗，显示狗狗
                            request.setAttribute("showdog", 1);
                        }
                        break;
                    case 1:// 狗狗已领养,显示开心农场
                        request.setAttribute("showdog", 2);
                        break;
                    case 2:// 农场使用,显示小游戏
                        request.setAttribute("showdog", 3);
                        break;
                    case 3:// 小游戏已使用
                        request.setAttribute("hidedog", "1");
                        break;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        } catch (Exception e) {
            cost.endException(e);
            e.printStackTrace();
        } finally {
            cost.endFinally();
        }
        try {
            BuildSchedule bu = new BuildSchedule(host);
            request.setAttribute("buildSchedule", bu);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return mapping.findForward(forward);
    }

    private void initUserCounts(HttpServletRequest request, User host) {
        try {
            List<UserCount> counters = UserCountMgr.getCounts(host.getId());
            if (counters != null) {
                for (UserCount uc : counters) {
                    if (uc != null) {
                        switch (uc.getType()) {
                            case UserCountMgr.POKE:
                                request.setAttribute("pokeCount", uc.getCount());
                                break;
                            case UserCountMgr.GUEST_REQUEST:
                                request.setAttribute("buddyApplyCount", uc.getCount());
                                break;
                            case UserCountMgr.REG_APPLY:
                                request.setAttribute("highSchoolApplyCount", uc.getCount());
                                break;
                            case UserCountMgr.APP_INVITE:
                                request.setAttribute("appApplyCount", uc.getCount());
                                break;
                            case UserCountMgr.RECOMMENDED_FRIEND:
                                request.setAttribute("recommendedCount", uc.getCount());
                                break;
                        }
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
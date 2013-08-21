package com.xiaonei.platform.core.opt.base.action;

import java.io.IOException;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;

/**
 * 
 * ActivatedAction.java
 * 
 * @author drizzt drizzt.wang@gmail.com 2006-6-26 2:16:37
 */
public abstract class ActivatedAction extends ProtectAction {

    public abstract ActionForward executeAt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws SQLException;

    public ActionForward executePt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws SQLException {
        //		User host = ThreadUtil.currentHost(request);
        //		if (host != null) {
        //			if (host.getStatus() == User.statusActivateVerified) {
        //				return redirectNewbie(request, response);
        //			} else if (host.getStatus() == User.statusActivatedVerify) {
        //				addMsg(session, DodoyoResource.getString("messages.action.verified.relay"));
        //				return UnverifiedSearchAction.redirectUnverifiedSearch(request, response);
        //			}
        //		}		
        com.xiaonei.platform.core.model.User usr = BaseThreadUtil.currentHost(request);

        if (usr != null && !UserConfirmLogic.getInstance().iNeedYou(usr)) { //校内才判断，开心不判断
            if (usr.getStatus() == User.statusActivateVerified) {
                return redirectNewbie(request, response);
            }
        }
        try {
            return executeAt(mapping, form, request, response);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static ActionForward redirectNewbie(HttpServletRequest request,
            HttpServletResponse response) {
        try {
            response.sendRedirect(OpiConstants.urlMain + "/Newbie.do");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}

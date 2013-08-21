package com.xiaonei.reg.guide.action;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.action.ProtectAction;


public class PopGameAction extends ProtectAction {

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws SQLException {
    	//据看好像是弹Pop一个游戏页用的，暂且留着吧~ 2010-2-9 11:55:27
    	//GuideLogger.printLog(" NOHERE! ",GuideLogger.ERROR);
        return mapping.findForward("succ");
    }

}

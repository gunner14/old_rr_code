package com.xiaonei.reg.guide.action;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.opt.base.action.BaseAction;


public class AjaxNoteAction extends BaseAction{
    public ActionForward executeBt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException{
        String divid=request.getParameter("divid");
        String userid=request.getParameter("userid");
        MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide); 
        mem.set("AjaxNoteAction"+divid+userid, "closed", (int)com.xiaonei.platform.core.cache.MemCacheKeys.hour*72);
        return null;
    }

}

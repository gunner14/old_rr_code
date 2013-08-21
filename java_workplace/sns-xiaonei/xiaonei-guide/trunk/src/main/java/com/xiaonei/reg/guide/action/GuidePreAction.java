package com.xiaonei.reg.guide.action;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;

/**
 * <ul>
 * <li>
 * 进入guide页之前的所有操作再这里进行
 *
 * @author skllb </li> <li>2009-2-18 下午03:26:41</li> <li></li>
 *         </ul>
 */
public class GuidePreAction extends ProtectAction {
    public ActionForward executePt(ActionMapping mapping, ActionForm form,HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        User host = BaseThreadUtil.currentHost(request);
        if (host != null && host.getId() % 2 != 0) {
            return mapping.findForward("regToDog");
        } else {
            return mapping.findForward("regToFarm");
        }
    }
}

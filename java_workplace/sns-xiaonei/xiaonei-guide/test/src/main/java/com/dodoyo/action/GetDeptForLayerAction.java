package com.dodoyo.action;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.datum.dao.DepartmentDAO;
import com.dodoyo.datum.model.Department;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;

/**
 * GetDeptForLayerAction
 * 
 * @author lifs
 */
public class GetDeptForLayerAction extends ProtectAction {
    public ActionForward executePt(ActionMapping mapping, ActionForm form,HttpServletRequest request, HttpServletResponse response)	throws SQLException {
		int univ = 0;
		String ver = "" + Calendar.getInstance().get(Calendar.YEAR)	+ (Calendar.getInstance().get(Calendar.MONTH) + 1) + Calendar.getInstance().get(Calendar.DATE);
		try {
			univ = Integer.parseInt(request.getParameter("id"));
		} catch (Exception ignored) {
		}
		String contx = this.servlet.getServletContext().getRealPath("/pages/dept/");
		File fl = new File(new StringBuilder().append(contx).append("/_").append(univ).append(".html").toString());
		if (fl.exists()) {
			long time = new Date().getTime() - fl.lastModified();
			if (time > 86400000L) {
				try {
					fl.delete();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		try {
			if (!fl.exists()) {
				try {
					fl.createNewFile();
				} catch (Exception e) {
					e.printStackTrace();
					return null;
				}
				FileWriter fw = new FileWriter(fl);
				String dname = GetDepAction.getEscapedString("院系");
				fw.write(new StringBuilder().append("<li><a onclick=\"if(cl_univdept){return cl_univdept(event)}\" href=\"#\">").append(dname).append("</a></li>\n").toString());
				List<Department> ls = DepartmentDAO.getInstance().getByUniv(univ);
				for (Department dept : ls) {
					if (dept.getName() != null && !"".equals(dept.getName().trim())) {
						dname = GetDepAction.getEscapedString(dept.getName());
						fw.write(new StringBuilder().append("<li><a onclick=\"if(cl_univdept){return cl_univdept(event)}\" href=\"#\">").append(dname).append("</a></li>\n").toString());
					}
				}
				dname = GetDepAction.getEscapedString("其它院系");
				fw.write(new StringBuilder().append("<li><a onclick=\"if(cl_univdept){return cl_univdept(event)}\" href=\"#\">").append(dname).append("</a></li>").toString());
				fw.close();				
			}
			response.sendRedirect(new StringBuilder().append("http://").append(request.getServerName()).append("/pages/dept/_").append(univ).append(".html?ver=").append(ver).toString());
		} catch (IOException e) {
			return null;
		}
		return null;
	}

}

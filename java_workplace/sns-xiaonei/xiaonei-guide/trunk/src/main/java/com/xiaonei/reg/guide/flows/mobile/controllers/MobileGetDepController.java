package com.xiaonei.reg.guide.flows.mobile.controllers;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.dodoyo.datum.dao.DepartmentDAO;
import com.dodoyo.datum.model.Department;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForm;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForward;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionMapping;
import com.xiaonei.reg.guide.util.GuideLogger;

//<action path="/GetDep" type="com.dodoyo.action.GetDepAction"  scope="request">

/**
 * MobileGetDepController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-2 新建文件
 */
@LoginRequired
@Path("GetDep.do")
public class MobileGetDepController {
    
    /*
     * <action path="/GetDep" type="com.dodoyo.action.GetDepAction" scope="request">
            <forward name="succ" path="/pages/getDep.jsp" />
        </action>
     */
    @Get
    @Post
    public String index(Invocation inv) {
        GuideLogger.printLog("");
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        Map<String, ActionForward> map = new HashMap<String, ActionForward>();
        map.put("succ", new ActionForward("/pages/getDep.jsp"));
        ActionMapping mapping = new ActionMapping(map);
        ActionForward fwd = executeBt(mapping, null, request, response,inv);
        if(fwd == null){
        	return "@";
        }
        return fwd.toString();
    }
    
	public ActionForward executeBt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response,Invocation inv)
			{
		User host = BaseThreadUtil.currentHost(request);
		int univ;
		String ver = "" + Calendar.getInstance().get(Calendar.YEAR)
				+ (Calendar.getInstance().get(Calendar.MONTH) + 1)
				+ Calendar.getInstance().get(Calendar.DATE);
		try {
			univ = Integer.parseInt(request.getParameter("id"));
		} catch (Exception e) {
			univ = UserAdapter.get(host).getUniv();
		}
		int ii = 0;
		try {
			ii = Integer.parseInt(request.getParameter("i"));
		} catch (Exception e) {
		}
		String index = "", indexPix = "";
		if (ii > 0) {
			index = String.valueOf(ii);
			indexPix = "_" + index;
		}

		University univer = NetworkManager.getInstance().getUnivById(univ);
		if (univer == null) {
			return null;
		}
		//String contx = this.servlet.getServletContext().getRealPath("/pages/dept/");
		String contx = inv.getServletContext().getRealPath("/pages/dept/");
		File fl = new File(contx + "/" + univ + indexPix + ".html");
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
				StringBuffer optionText = new StringBuffer();
				optionText
						.append("<select id='department"
								+ index
								+ "' name='department"
								+ index
								+ "' class='select' tabindex='6' onchange='changeDept()'>\n");
				optionText.append("<option value=''>" + getEscapedString("院系")
						+ "</option>\n");
				List<Department> DeptLs = DepartmentDAO.getInstance()
						.getByUniv(univ);
				for (Department dept : DeptLs) {
					if (dept.getName() != null
							&& !"".equals(dept.getName().trim()))
						optionText.append("<option value='"
								+ getEscapedString(dept.getName()) + "'>"
								+ getEscapedString(dept.getName())
								+ "</option>\n");
				}
				optionText.append("<option value='其它院系'>其它院系</option>");
				optionText.append("</select>\n");

				FileWriter fw = null;
				try {
					try {
						fl.createNewFile();
					} catch (Exception e) {
						e.printStackTrace();
					}
					fw = new FileWriter(fl);
					fw.write(optionText.toString());
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					if (null != fw)
						fw.close();
				}
				response.getWriter().print(optionText.toString());
			}

			if (fl.exists() && fl.canRead()) {
				return new ActionForward("/pages/dept/" + univ + indexPix
						+ ".html?ver=" + ver, false);
			}
		} catch (IOException e) {
			return null;
		} catch (SQLException e) {
			return null;
		} 
		catch (Exception e) {
			System.err.print("MobileGetDepController "+e.toString());
			return null;
		} 

		return null;
	}

	public static String getEscapedString(String source) {
		StringBuffer result = new StringBuffer();
		for (int i = 0; i < source.length(); i++) {
			char c = source.charAt(i);
			if ((c) < 256)
				result.append(c);
			else
				result.append("&#" + (int) c + ";");
		}
		return result.toString();
	}
}

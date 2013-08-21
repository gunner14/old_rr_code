package com.xiaonei.reg.guide.action.netbarflow;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.RegPortraitLogic;


public class PortraitInitAction extends ProtectAction{

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		GuideLogger.printLog(" - host:"+ host.getId()+ " - start" +" - current GDomain is:"+Globals.urlGuide);
		cost.endStep();
		try {
			request.setAttribute("sex_flag", UserAdapter.get(host).getGender());
		} catch (Exception e) {
			GuideLogger.printLog("setAttribute failed! - host:"+ host.getId(),GuideLogger.ERROR);
		}
		cost.endStep();
		// 跳转到head.upload域下
		String sex_flag = "1";
		if (User.genderFemale.equalsIgnoreCase(UserAdapter.get(host).getGender())) {
			sex_flag = "0";
		}
		cost.endStep();
		
		boolean hasHead = true;
		try {
			hasHead = RegPortraitLogic.hasUploaded(host);
		} catch (Exception e) {
			GuideLogger.printLog("hasUploaded failed! - host:"+ host.getId(),GuideLogger.ERROR);
		}
		if(hasHead){
			GuideLogger.printLog("initPage() - host:"+ host.getId()+ " - end - to guide - head = "+UserAdapter.get(host).getHeadUrl());
			cost.endStep();
			try {
				//succOk(request, null, mapping, null);
			} catch (Exception e) {
				GuideLogger.printLog("succOk failed! - host:"+ host.getId(),GuideLogger.ERROR);
			}
			cost.endStep();
			return new ActionForward(Globals.urlGuide+"/guide.do",true);
		}
		cost.endStep();
		
		GuideLogger.printLog("initPage() - host:"+ host.getId()+ " - end - sex="+sex_flag);
		cost.endFinally();
		
		String skipurl = Globals.urlGuide + "/ponbs.do?skip=1";
		try {
			skipurl = URLEncoder.encode(Globals.urlGuide + "/ponbs.do?skip=1","UTF-8");
		} catch (UnsupportedEncodingException e) {
			GuideLogger.printLog("Encoding error! host:"+host.getId()+" "+e.toString());
			e.printStackTrace();
		} 
		//http://head.upload.renren.com/registerguide/EntryBar.do
		String headurl = "http://head.upload."+Globals.domain+"/registerguide/EntryBar.do?rurl="
		+ skipurl + "&sex_flag="
		+ sex_flag;
		GuideLogger.printLog("initPage() - host:"+ host.getId()+ " - headurl = "+headurl);
		return new ActionForward(headurl, true);
	}

}

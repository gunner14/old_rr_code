package com.xiaonei.reg.usertrace.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * 日志记录，开放给其他人的http调用接口 目前在使用 1.自动激活2.msntab 2008-9-18
 * 
 * @author wei.cheng@opi-corp.com
 */

@Path({"userTraceLog","userTraceLog.do", "ajxutlog"})
public class HTTPTraceLogController {

	private static final String ADMIN_MAIL_ACTIVTE_USER_SS = "admin_active";
	private static final String MSNTAB_VIEW_SS = "msn_tab";

	@Get
	@Post
	public String executeBt(Invocation invocation,
			@Param("email") String email, @Param("ss") String s1,
			@Param("s2") String s2, @Param("s3") String s3,
			@Param("s4") String s4, @Param("i") int userId,
			@Param("stage") int stage, @Param("action_id") String action_id) {
		HttpServletRequest req = invocation.getRequest();

		if (ADMIN_MAIL_ACTIVTE_USER_SS.equals(s1)) {
			UserTraceLogic.getInstance().insertRegHisForAdmin(userId, email,stage, "", s2, s3, s4);
		} else if (MSNTAB_VIEW_SS.equals(s1)) {
			UserTraceLogic.getInstance().insertVisitHisFirst(req,String.valueOf(RegOprDefine.MSNTAB_VIEW), "msn_tab", null,null, null, 0);
		} else {
			UserTraceLogic.getInstance().insertVisitHisFirst(req, action_id, s1, s2, s3, s4, userId);
		}
		return null;
	}

}

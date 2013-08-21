package com.xiaonei.reg.register.controllers.ajax;

import java.io.PrintWriter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserBasicAdapter;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * AjaxGuideFlowLogAction
 * <p/>
 * guide 流程中的log<br>
 * 高中的preview页，点击跳过和加为好友的log
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-27 - 下午05:19:10
 */

@Path({ "AjaxGuideFlowLog" })
public class AjaxGuideFlowLogController {

	private static final String PAR_ACT = "act";
	private static final String PAR_FRIENDS_TOTAL_COUNT = "ftc";
	private static final String PAR_REQUEST_COUNT = "rc";
	private static final String ACT_PREVIEW_SKIP = "gphs";
	private static final String ACT_PREVIEW_ADD_FRIEND = "gpha";
	private static final String ACT_IMPORTMSN_SKIP = "is";
	private static final String ACT_IMPORTMSN_IMPORT = "ii";
	private static final String ACT_CIN_SKIP = "cs";
	private static final String ACT_CIN_SKIP_NOIN = "csn";
	private static final String ACT_CIN_ADD_FRIEND = "caf";
	private static final String ACT_CNOTIN_SKIP = "cns";
	private static final String ACT_CNOTIN_INVITE = "cni";

	private static final String ACT_SKIP_DOG = "sdog";
	private static final String ACT_ADD_DOG = "adog";

	private static final String ACT_SKIP_FARM = "sfarm";
	private static final String ACT_ADD_FARM = "afarm";

	@Get
	@Post
	public String executePt(Invocation invocation) {
		HttpServletRequest request = invocation.getRequest();
		HttpServletResponse response = invocation.getResponse();
		User host = BaseThreadUtil.currentHost(request);
		UserBasic ubc = WUserBasicAdapter.getInstance().get(host.getId());

		if (StringUtils.equals(request.getParameter(PAR_ACT), ACT_PREVIEW_SKIP)) {
			tracelog(request, host, RegOprDefine.PREVIEW_SKIP, null, null);
			return null;
		}
		//
		else if (StringUtils.equals(request.getParameter(PAR_ACT),
				ACT_PREVIEW_ADD_FRIEND)) {
			tracelog(request, host, RegOprDefine.PREVIEW_ADD_FRIEND, null, null);
			return null;
		}
		//
		else if (StringUtils.equals(request.getParameter(PAR_ACT),
				ACT_IMPORTMSN_SKIP)) {
			tracelog(request, host, RegOprDefine.IMPORT_MSN_SKIP, null, null);
			return null;
		}
		//
		else if (StringUtils.equals(request.getParameter(PAR_ACT),
				ACT_IMPORTMSN_IMPORT)) {
			tracelog(request, host, RegOprDefine.IMPORT_MSN_ADD_FRIEND, null,
					null);
			return null;
		}
		//
		else if (StringUtils
				.equals(request.getParameter(PAR_ACT), ACT_CIN_SKIP)) {
			tracelog(request, host, RegOprDefine.IMPORT_MSN_CIN_SKIP, null,
					null);
			return callCompleted(response);
		}
		//
		else if (StringUtils.equals(request.getParameter(PAR_ACT),
				ACT_CIN_SKIP_NOIN)) {
			tracelog(request, host, RegOprDefine.IMPORT_MSN_CIN_SKIP_NOTIN,
					null, null);
			return callCompleted(response);
		}
		//
		else if (StringUtils.equals(request.getParameter(PAR_ACT),
				ACT_CIN_ADD_FRIEND)) {
			// 申请数，好友总数
			String requestCount = request.getParameter(PAR_REQUEST_COUNT);
			String friendTotalCount = request
					.getParameter(PAR_FRIENDS_TOTAL_COUNT);
			tracelog(request, host, RegOprDefine.IMPORT_MSN_CIN_ADD_FRIEND,
					friendTotalCount, requestCount);
			return callCompleted(response);
		}
		//
		else if (StringUtils.equals(request.getParameter(PAR_ACT),
				ACT_CNOTIN_SKIP)) {
			tracelog(request, host, RegOprDefine.IMPORT_MSN_CNOTIN_SKIP, null,
					null);
			return callCompleted(response);
		}
		//
		else if (StringUtils.equals(request.getParameter(PAR_ACT),
				ACT_CNOTIN_INVITE)) {
			// 申请数，好友总数
			String requestCount = request.getParameter(PAR_REQUEST_COUNT);
			String friendTotalCount = request
					.getParameter(PAR_FRIENDS_TOTAL_COUNT);
			tracelog(request, host, RegOprDefine.IMPORT_MSN_CNOTIN_INVITE,
					friendTotalCount, requestCount);
			return callCompleted(response);
		} else if (ACT_ADD_DOG.equals(request.getParameter(PAR_ACT))) {
			// 领养了狗狗

			UserTraceLogic.getInstance().insertRegHis(request, host,
					host.getAccount(),
					String.valueOf(RegOprDefine.GUIDE_HS_ADD_DOG),
					ubc.getStage(), null, null, null, null, 0);
			return null;

		} else if (ACT_SKIP_DOG.equals(request.getParameter(PAR_ACT))) {
			// 跳过了狗狗领养
			UserTraceLogic.getInstance().insertRegHis(request, host,
					host.getAccount(),
					String.valueOf(RegOprDefine.TEM_NOT_FOSTER),
					ubc.getStage(), null, null, null, null, 0);
			return null;
		} else if (ACT_SKIP_FARM.equals(request.getParameter(PAR_ACT))) {
			// 跳过了农场
			UserTraceLogic.getInstance().insertRegHis(request, host,
					host.getAccount(),
					String.valueOf(RegOprDefine.GUIDE_HS_SKIP_FARM),
					ubc.getStage(), null, null, null, null, 0);
			return null;
		}
		if (ACT_ADD_FARM.equals(request.getParameter(PAR_ACT))) {
			// 进入了农场
			UserTraceLogic.getInstance().insertRegHis(request, host,
					host.getAccount(),
					String.valueOf(RegOprDefine.GUIDE_HS_ADD_FARM),
					ubc.getStage(), null, null, null, null, 0);
			return null;
		}

		return null;
	}

	private void tracelog(HttpServletRequest request, User host, int aLog,
			String s3, String s4) {
		UserTraceLogic.getInstance().insertRegHis(request, host,
				host.getAccount(), String.valueOf(aLog),
				WUserBasicAdapter.getInstance().get(host.getId()).getStage(),
				null, null, s3, s4, 0);
	}

	private String callCompleted(HttpServletResponse response) {
		response.setContentType("text/html;charset=utf-8");
		try {
			PrintWriter out = response.getWriter();
			out.print("<script>document.domain=\"" + Globals.domain
					+ "\";parent.onLogCompleted();</script>");
		} catch (Exception e) {
			LogTimeUtils.logger.error(e);
		}
		return null;
	}

}

package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.page.xoa.domain.CheckResultView;

@Path("as")
public class AntiSpamController {
	
	private final static Logger logger = Logger.getLogger("PAGEXOA");


	@Get
	public CheckResultView index(@Param("opUserId") int opUserId,
			@Param("content") String content,
			@Param("checkType") int checkType,
			@Param("opUserIp") String opUserIp) {
		
		logger.info("opUserId:"+opUserId);
		logger.info("content:"+content);
		logger.info("checkTpye:"+checkType);
		logger.info("opUserIp:"+opUserIp);
		
		CheckResult checkResult = AntiSpamAdapter.getInstance()
		.checkAndFilter(opUserId, checkType, content,
				null);
		if(checkResult!=null){
			CheckResultView checkResultView=new CheckResultView();
			
			checkResultView.setFlag(checkResult.getFlag());
			checkResultView.setAdvertisement(checkResult.isAdvertisement());
			checkResultView.setAuditFirst(checkResult.isAuditFirst());
			checkResultView.setHighLightContent(checkResult.getHighLightContent());
			checkResultView.setInfoType(checkResult.getInfoType());
			checkResultView.setNotNotify(checkResult.isNotNotify());
			checkResultView.setPreContent(content);
			checkResultView.setSafeContent(checkResult.getSafePureContent());
			checkResultView.setTimeout(checkResult.isTimeout());
			checkResultView.setShowMessage(checkResult.getShowMessage());
			
			return checkResultView;
		}
		return null;
	}
}

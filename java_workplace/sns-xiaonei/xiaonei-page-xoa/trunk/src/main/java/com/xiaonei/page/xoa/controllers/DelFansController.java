package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.remotelog.LogEntry;
import com.xiaonei.page.remotelog.LogEntryUtil;
import com.xiaonei.page.remotelog.LogType;
import com.xiaonei.page.service.FansService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("dfs")
public class DelFansController {

	@Autowired
	private FansService fansService;
	
	@Autowired
	private PageService pageService;
	
	private final static Logger logger=Logger.getLogger("PAGEXOA");
	
	@Get
	public String index(@Param("pageId")int pageId,@Param("userId")int userId){
		Page page=null;
		User user = null;
		try{
			page=pageService.get(pageId);
			user = WUserAdapter.getInstance().get(userId);
		}catch (Exception e) {
			logger.error(e.getMessage(), e);
			return "@-1";
		}
		if(page!=null&&userId!=0){
			//为什么返回void，怎么知道失败了
			try{
			    LogEntry remoteLog = LogEntryUtil.buildLogEntry(page, user, pageId, null,
		                        LogType.DEL_FANS, null);
			    remoteLog.setUserAgent("TYPE_XOA");
				fansService.delFans(page,userId,remoteLog);
			}catch(PageException e){
				logger.error(String.format("delFans (Page=%s, User=%s)", page.getId(), userId), e);
				//ignore this exception
				return "@-1";
			}
		}
		return "@0";
	}
}

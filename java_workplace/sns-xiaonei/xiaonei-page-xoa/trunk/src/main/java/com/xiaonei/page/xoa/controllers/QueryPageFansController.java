package com.xiaonei.page.xoa.controllers;

import java.sql.SQLException;
import java.util.LinkedList;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;

import com.xiaonei.page.dao.PageFansDAO;
import com.xiaonei.page.xoa.domain.UserView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("qpfs")
public class QueryPageFansController {

//	@Autowired
//	private FansService fansService;
//	
//	@Autowired
//	private PageService pageService;
	
	private final static Logger logger=Logger.getLogger("PAGEXOA");
	
	
	@Get
	public UserView[] index(@Param("pageId") int pageId,
			@Param("offset") int offset, @Param("limit") int limit) {
		List<UserView> fansUserView=new LinkedList<UserView>();
		try {
			List<Integer> fans=PageFansDAO.getInstance(false).getFansUserIdList(pageId, offset,limit);
			
			if(fans!=null){
				logger.info(fans.size());
				for (Integer fanId : fans) {
					UserView uv=new UserView();
					User user=null;
					try{
						user=WUserAdapter.getInstance().get(fanId);
					}catch (Exception e) {
						logger.error("Get user failed!", e);
						//ignore
					}		
					if(user!=null){
						uv.setAccount(user.getAccount());
						uv.setAuth(user.getAuth());
						uv.setDomain(user.getDomain());
						uv.setHeadFullUrl(user.getHeadFullUrl());
						uv.setHeadUrl(user.getHeadUrl());
						uv.setId(user.getId());
						uv.setLargeFullUrl(user.getLargeFullUrl());
						uv.setLargeUrl(user.getLargeUrl());
						uv.setLevel(user.getLevel());
						uv.setLoginCount(user.getLoginCount());
						uv.setMainFullUrl(user.getMainFullUrl());
						uv.setMainUrl(user.getMainUrl());
						uv.setNickName(user.getNickname());
						uv.setRights(user.getRights());
						uv.setStar(user.getStar());
						uv.setStatus(user.getStatus());
						uv.setTinyFullUrl(user.getTinyFullUrl());
						uv.setTinyUrl(user.getTinyUrl());
						uv.setName(user.getName());
					}
					fansUserView.add(uv);
				}
				
				return fansUserView.toArray(new UserView[fans.size()]);
			}
			return null;
		} catch (SQLException e) {
			logger.error("DB failed!", e);
			return null;
		}
		
	}
}

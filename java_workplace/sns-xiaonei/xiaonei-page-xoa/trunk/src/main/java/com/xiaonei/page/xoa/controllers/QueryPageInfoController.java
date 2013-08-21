package com.xiaonei.page.xoa.controllers;

import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.dao.PageInfoDAO;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageInfo;
import com.xiaonei.page.domain.PageTypeInfoMapping;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.info.InfoParserFactory;
import com.xiaonei.page.service.info.InfoTypeService;
import com.xiaonei.page.xoa.domain.PageInfoView;

@Path("qpi")
public class QueryPageInfoController {

	@Autowired
	private InfoTypeService infoTypeService;

	@Autowired
	private PageService pageService;

	@Autowired
	protected InfoParserFactory parserFactory;
	
	private final static Logger logger = Logger.getLogger("PAGEXOA");

	@Get
	@Post
	public PageInfoView index(@Param("pageId") int pageId) {
		PageInfo pageInfo = null;
		Page page = null;
		try {
			pageInfo = PageInfoDAO.getInstance().get(pageId);
			page = pageService.get(pageId);
			Map<Short, List<PageTypeInfoMapping>> result = infoTypeService
					.getInfoTypeMapping(page);
			Set<Entry<Short, List<PageTypeInfoMapping>>> iter = result
					.entrySet();
			for (Entry<Short, List<PageTypeInfoMapping>> entry : iter) {
				logger.info("key:" + entry.getKey());

				List<PageTypeInfoMapping> list = entry.getValue();
				for (PageTypeInfoMapping pageTypeInfoMapping : list) {
					logger.info("id:" + pageTypeInfoMapping.getId()
							+ " infoname:" + pageTypeInfoMapping.getInfoName()
							+ " infoType" + pageTypeInfoMapping.getInfoType());
				}
				logger.info("***************************************");
			}
			

			if (pageInfo != null) {
				PageInfoView piv = new PageInfoView();

				piv.setAddTime(pageInfo.getAddTime());
				//piv.setBasic(pageInfo.getBasic());
				String pageInfoBasic=pageInfo.getBasic();
				if(pageInfoBasic!=null&&pageInfoBasic!=""){
					piv.setBasic(repalceInt(pageInfo.getBasic(), result.get((short) 1)));
				}
				String pageInfoContact=pageInfo.getContact();
				if(pageInfoContact!=null&&pageInfoContact!=""){
					piv.setContact(repalceInt(pageInfo.getContact(), result.get((short) 3)));
				}
				
				//piv.setContact(pageInfo.getContact());
				//piv.setDetail(pageInfo.getDetail());
				piv.setPageId(pageInfo.getPageId());
				piv.setPtype(pageInfo.getPtype());
				piv.setUpdateTime(pageInfo.getUpdateTime());
				
				String pageInfoDetail=pageInfo.getDetail();
				if(pageInfoDetail!=null&&pageInfoDetail!=""){
					piv.setDetail(repalceInt(pageInfo.getDetail(), result.get((short) 2)));
				}
				return piv;
			}
		} catch (Exception e) {
			// ignore
			logger.error(e.getMessage(), e);
		}
		return null;
	}
	
	// TODO 这个方法挺恶的。fixit！
	protected String repalceInt(String str, List<PageTypeInfoMapping> list) {
		for (PageTypeInfoMapping pageTypeInfoMapping : list) {
			String integerMark = String.valueOf(pageTypeInfoMapping
					.getInfoType());
			if (str.indexOf(integerMark) != -1) {
				logger.info("true:"+pageTypeInfoMapping.getInfoType()+"|"+pageTypeInfoMapping.getInfoName());
				str=str.replace(integerMark, pageTypeInfoMapping.getInfoName());
			}
		}
		return str;
	}

}

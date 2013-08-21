package com.xiaonei.page.xoa.controllers;

import java.util.LinkedList;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.codehaus.jackson.map.ObjectMapper;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.xoa.domain.PageView;
import com.xiaonei.page.xoa.util.PageXoaUtil;

@Path("qps")
public class QueryPagesController {

	@Autowired
	private PageService pageService;
	
	private final static ObjectMapper mapper = new ObjectMapper();
	
	private final static Logger logger = Logger.getLogger("PAGEXOA");

	@Get
	public PageView[] index(@Param("offset") int offset, @Param("limit") int limit,
			@Param("subType") int subType, @Param("orderBy") String orderBy,
			@Param("orderType") String orderType) {
		List<Page> pages = pageService.getPagesBySubType(subType, offset,
				limit, orderBy, orderType);

		if(pages!=null){
			
			int size=pages.size();
			logger.info("get pages:"+size);
			PageView[] pagesView=new PageView[size];
			for (int index=0;index<size ; index++) {
				Page page=pages.get(index);
				PageView pv=new PageView();
				
				pv.setAgeRestriction(page.getAgeRestriction());
				pv.setAlias(page.getAlias());
				pv.setApply_time(page.getApply_time());
				pv.setChecked(page.isChecked());
				pv.setCounta(page.getCounta());
				pv.setCreateTime(page.getCreateTime());
				pv.setDescription(page.getDescription());
				pv.setFansCount(page.getFansCount());
				pv.setHasIncrement(page.isHasIncrement());
				pv.setHeadUrl(page.getHeadUrl());
				pv.setId(page.getId());
				pv.setIncrementNumHigh(page.getIncrementNumHigh());
				pv.setIncrementNumLow(page.getIncrementNumLow());
				pv.setIncrementNumMid(page.getIncrementNumMid());
				pv.setLargeUrl(page.getLargeUrl());
				pv.setLevel(page.getLevel());
				pv.setMainUrl(page.getMainUrl());
				pv.setName(page.getName());
				pv.setOriType(page.getOriType());
				pv.setOwnerId(page.getOwnerId());
				pv.setState(page.getState());
				pv.setSubType(page.getSubType());
				pv.setSubTypeString(page.getSubTypeString());
				pv.setTinyUrl(page.getTinyUrl());
				pv.setType(page.getType());
				pv.setTypeString(page.getTypeString());
				pv.setVisibility(page.getVisibility());
				
				pagesView[index]=pv;
			}

			
			return pagesView;
		}
		return null;
	}
}

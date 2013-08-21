package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.gid.util.G;
import com.xiaonei.commons.gid.util.Type;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.xoa.domain.PageView;

@Path("qp")
public class QueryPageController {
	
	@Autowired
	private PageService pageService;
	
	@Get
	@Post
	public PageView index(@Param("pageId")int pageId){
		Page page=pageService.get(pageId);
		
		PageView pv=new PageView();

		if(page!=null){
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
			
			return pv;
		}
		
		return null;
	}
	
	
    @Get("ispage")
    @Post("ispage")
    public boolean isPage(@Param("page-id") int pageId) {
        if (G.isTypeOf(pageId, Type.PAGE)) {
            return true;
        }
        return false;
    }
}

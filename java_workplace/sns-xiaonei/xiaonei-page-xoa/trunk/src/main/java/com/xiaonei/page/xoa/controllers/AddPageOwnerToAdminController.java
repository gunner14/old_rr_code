package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AdminService;
import com.xiaonei.page.service.PageService;

@Path("apota")
public class AddPageOwnerToAdminController {

	@Autowired
	private AdminService adminService;

	@Autowired
	private PageService pageService;

	private final static Logger logger = Logger.getLogger("PAGEXOA");

	@Get
	public String index(@Param("ownerId") int ownerId,
			@Param("pageId") int pageId) {
		Page page;
		try {
			page = pageService.get(pageId);
			int currentOwnerId = page.getOwnerId();
			if (ownerId != currentOwnerId) {
				return "@NOT_PAGE_OWNER";
			}
			adminService.addAdmin(page, ownerId, ownerId);
		} catch (PageException e) {
			logger.error(e.getMessage(), e);
			return "@false";
		}
		return "@true";
	}
}

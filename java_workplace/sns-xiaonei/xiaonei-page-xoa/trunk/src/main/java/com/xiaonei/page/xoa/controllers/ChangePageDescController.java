package com.xiaonei.page.xoa.controllers;

import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.page.PageException;
import com.xiaonei.page.dao.PageInfoDAO;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageInfo;
import com.xiaonei.page.domain.PageTypeInfoMapping;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.PageInfoService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.info.InfoTypeService;
import com.xiaonei.page.util.Util;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("cpd")
public class ChangePageDescController {

	@Autowired
	private PageService pageService;

	@Autowired
	private InfoTypeService infoTypeService;

	@Autowired
	private AuthService authService;

	@Autowired
	private PageInfoService pageInfoService;

	private final static String DESCRIPTIONCODE = "20088";

	private final static Logger logger = Logger.getLogger("PAGEXOA");

	/**
	 * 代码中写死，简介-->20088；联系电话-->20093;详细地址-->20102.
	 * 
	 * @param pageId
	 * @param opUserId
	 * @param desc
	 * @return
	 */
	@Get
	@Post
	public String index(@Param("pageId") int pageId,
			@Param("opUserId") int opUserId, @Param("desc") String desc) {
		if (Util.isEmpty(desc)) {
			return "@CONTENT_EMPTY";
		}
		User host = WUserAdapter.getInstance().get(opUserId);
		// 没有权限
		try {
			if (!authService.isPageAdministrator(host, pageId)) {
				logger.info(String.format("%d not admin of page: %d",
						host.getId(), pageId));
				return "@NOT_PAGE_ADMIN";
			}
		} catch (PageException e) {
			logger.error(e.getMessage(), e);
			return "@DB_ERROR";
		}

		// CheckResult checkResult =
		// AntiSpamAdapter.getInstance().checkAndFilter(
		// opUserId, CheckType.PAGE_INFO_AND_NETWORK, desc,null);
		CheckResult checkResult = AntiSpamAdapter.getInstance().checkAndFilter(
				opUserId, CheckType.PAGE_INFO_TYPE, desc, null);
		logger.info("anti type:" + checkResult.getFlag());
		if (checkResult.getFlag() == CheckResult.PROHIBITED) {
			return "@PROHIBITED";
		}

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
				String pageInfoDetail = pageInfo.getDetail();
				JSONObject detail = null;
				if (pageInfoDetail != null && pageInfoDetail != "") {
					logger.info("old detail:" + pageInfoDetail);
					detail = JSONObject.fromObject(pageInfoDetail);
				} else {
					detail = new JSONObject();
				}
				
				detail.put(DESCRIPTIONCODE, desc);
				pageInfo.setDetail(detail.toString());
				logger.info("new detail:" + detail.toString());
				// 更新
				pageInfoService.update(pageInfo);
			} else {//pageInfo为空
				logger.info("PAGE INFO IS NULL!");
				pageInfo = new PageInfo();
				pageInfo.setPageId(pageId);
				JSONObject detail = new JSONObject();
				detail.put(DESCRIPTIONCODE, desc);
				pageInfo.setDetail(detail.toString());
				logger.info("Before insert:" + pageInfo == null);
				// 插入
				pageInfoService.insert(pageInfo);

			}
		} catch (Exception e) {
			// ignore
			logger.error(e.getMessage(), e);
		}

		return "@" + pageId;
	}
}

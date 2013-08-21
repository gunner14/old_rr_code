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

@Path("cpc")
public class ChangePageContactController {

	@Autowired
	private PageService pageService;

	@Autowired
	private InfoTypeService infoTypeService;

	@Autowired
	private AuthService authService;

	@Autowired
	private PageInfoService pageInfoService;

	private final static String ADDRESSCODE = "20102";

	private final static String PHONECODE = "20093";

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
			@Param("opUserId") int opUserId, @Param("address") String address,
			@Param("phone") String phone) {

		if (Util.isEmpty(address) && Util.isEmpty(phone)) {
			return "@BOTH_FILED_EMPTY";
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

		CheckResult checkResultAddress = AntiSpamAdapter.getInstance()
				.checkAndFilter(opUserId, CheckType.PAGE_INFO_TYPE, address,
						null);
		CheckResult checkResultPhone = AntiSpamAdapter.getInstance()
				.checkAndFilter(opUserId, CheckType.PAGE_INFO_TYPE, address,
						null);
		logger.info("anti type:" + checkResultAddress.getFlag());
		if (checkResultAddress.getFlag() == CheckResult.PROHIBITED
				|| checkResultPhone.getFlag() == CheckResult.PROHIBITED) {
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
				String pageInfoContact = pageInfo.getContact();
				JSONObject contact = null;
				if (pageInfoContact != null && pageInfoContact != "") {
					logger.info("old contect:" + pageInfoContact);
					contact = JSONObject.fromObject(pageInfoContact);
				} else {
					logger.info("pageInfo Contact is null!");
					contact = new JSONObject();
				}

				contact.put(ADDRESSCODE, address);
				contact.put(PHONECODE, phone);
				pageInfo.setContact(contact.toString());

				logger.info("new contect:" + contact.toString());
				// 更新
				pageInfoService.update(pageInfo);
			} else {// pageInfo为空
				logger.info("PAGE INFO IS NULL!");
				pageInfo = new PageInfo();
				pageInfo.setPageId(pageId);
				JSONObject contact = new JSONObject();
				contact.put(ADDRESSCODE, address);
				contact.put(PHONECODE, phone);
				pageInfo.setContact(contact.toString());
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

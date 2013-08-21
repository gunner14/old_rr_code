package com.renren.fmwidget.controllers.widget;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.fmwidget.services.SetupWidget;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel;

@LoginRequired
public class StoreController {

	@Autowired
	private HostHolder host;

	@Autowired
	private SetupWidget widgetService;

	@Get
	public Object index(Invocation inv, @Param("page") int page) {
		int offset = page < 1 ? 0 : page - 1;
		List<FakemacWidgetDictModel> apps = widgetService.getShopApps(host.getUser().getId(), offset);
		JSONArray ja = new JSONArray();
		for (FakemacWidgetDictModel object : apps) {
			try {
				ja.add(JSONObject.fromObject(object.getApp_description()));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return ja;
	}
}

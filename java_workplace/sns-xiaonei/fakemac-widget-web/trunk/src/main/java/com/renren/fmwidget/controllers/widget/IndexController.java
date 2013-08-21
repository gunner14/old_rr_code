package com.renren.fmwidget.controllers.widget;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.fmwidget.enums.Apps;
import com.renren.fmwidget.model.AbstractCustomDatas;
import com.renren.fmwidget.model.AbstractModel;
import com.renren.fmwidget.model.AbstractSystemDatas;
import com.renren.fmwidget.model.Pos;
import com.renren.fmwidget.services.SetupWidget;
import com.renren.fmwidget.services.WidgetDefaultConfigService;
import com.renren.fmwidget.utils.JSONCover;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel;

@LoginRequired
@Path("")
public class IndexController {

	@Autowired
	private HostHolder host;

	@Autowired
	private SetupWidget widgetService;

	@Autowired
	private WidgetDefaultConfigService configService;

	/**
	 * index<br>
	 * 
	 * @param inv
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Sep 16, 2010 - 12:37:56 PM
	 */
	@Get
	public Object index(Invocation inv) {
		JSONObject ja = new JSONObject();
		List<AbstractModel<?, ?>> all = widgetService.selectAllByUserId(host
				.getUser().getId());
		for (AbstractModel<?, ?> abstractModel : all) {
			JSONObject fromObject = JSONCover.fromObject(abstractModel);
			ja.put(abstractModel.getSystemDatas().getId(), fromObject);
		}
		return ja;
	}

	/**
	 * delete<br>
	 * 
	 * @param inv
	 * @param uid
	 * 
	 * @author tai.wang@opi-corp.com Sep 16, 2010 - 12:38:02 PM
	 */
	@Get("del/{uid:\\d+}/")
	public void delete(Invocation inv, @Param("uid") int uid) {
		widgetService.delete(host.getUser().getId(), uid);
	}

	/**
	 * addition<br>
	 * 
	 * @param inv
	 * @param appName
	 * 
	 * @author tai.wang@opi-corp.com Sep 16, 2010 - 12:37:52 PM
	 */
	@Get("add/{appId:\\d+}/")
	public Object addition(Invocation inv, @Param("appId") int appId) {

		Apps app = null;
		if (null == (app = configService.getApp(appId))) {
			return "@{msg:\"no such app\"}";
		}
		int userId = host.getUser().getId();
		int uid = -1;
		try {
			uid = widgetService.install(userId, app);
		} catch (Exception e) {
			e.printStackTrace();
			return "@{msg:\"no such app\"}";
		}

		return -1 != uid ? uid : "@{msg:\"no such app\"}";
	}

	/**
	 * saveContent<br>
	 * 
	 * @param inv
	 * @param appId
	 * @param uid
	 * @param data
	 * 
	 * @author tai.wang@opi-corp.com Sep 16, 2010 - 2:08:33 PM
	 */
	@Post("{appId:\\d+}/{uid:\\d+}/save/content/")
	public void saveContent(Invocation inv, @Param("appId") int appId,
			@Param("uid") int uid, @Param("data") String data) {
		Apps app = configService.getApp(appId);
		if (null == app) {
			return;
		}
		int userId = host.getUser().getId();
		AbstractCustomDatas contentModel;
		JSONObject jo;
		try {
			jo = JSONObject.fromObject(data);
		} catch (Exception e) {
			return;
		}
		contentModel = JSONCover.toObject(jo, app.getCustomDatas());
		if (null == contentModel) {
			return;
		}
		widgetService.updateContent(userId, uid, contentModel);
	}

	/**
	 * saveConfig<br>
	 * 
	 * @param inv
	 * @param appId
	 * @param uid
	 * @param data
	 * 
	 * @author tai.wang@opi-corp.com Sep 16, 2010 - 2:08:38 PM
	 */
	@Post("{appId:\\d+}/{uid:\\d+}/save/config/")
	public void saveConfig(Invocation inv, @Param("appId") int appId,
			@Param("uid") int uid, @Param("data") String data) {
		Apps app = configService.getApp(appId);
		if (null == app) {
			return;
		}
		int userId = host.getUser().getId();
		AbstractSystemDatas configModel;
		JSONObject jo;
		try {
			jo = JSONObject.fromObject(data);
		} catch (Exception e) {
			return;
		}
		configModel = JSONCover.toObject(jo, app.getSystemDatas());
		if (null == configModel) {
			return;
		}
		widgetService.updateConfig(userId, uid, configModel);
	}

	/**
	 * savePosition<br>
	 * 
	 * @param inv
	 * @param appId
	 * @param uid
	 * @param data
	 * 
	 * @author tai.wang@opi-corp.com Sep 16, 2010 - 2:08:41 PM
	 */
	@Post("{appId:\\d+}/{uid:\\d+}/save/position/")
	public void savePosition(Invocation inv, @Param("appId") String appId,
			@Param("uid") int uid, @Param("data") String data) {
		int userId = host.getUser().getId();

		try {
			data = URLDecoder.decode(data, "UTF-8");
		} catch (UnsupportedEncodingException e1) {
			e1.printStackTrace();
			return;
		}
		JSONObject jo;
		try {
			jo = JSONObject.fromObject(data);
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
		Pos positionModel = JSONCover.toObject(jo, Pos.class);
		widgetService.updatePosition(userId, uid, positionModel);
	}

	@Post("/batch/save")
	public void bsave(Invocation inv, @Param("j") String sJson) {
		JSONArray ja;
		try {
			ja = JSONArray.fromObject(sJson);
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
		List<FakemacWidgetRecordModel> models = new ArrayList<FakemacWidgetRecordModel>();
		for (Iterator<?> iter = ja.iterator(); iter.hasNext();) {
			JSONObject jo = (JSONObject) iter.next();
			if (joHasName(jo,("systemDatas"))) {
				JSONObject joSD = jo.getJSONObject("systemDatas");
				if (joHasName(joSD, ("id")) && joHasName(joSD, ("typeNum"))) {
					int id = joSD.getInt("id");
					if (id < 0) {
						continue;
					}
					int typeNum = joSD.getInt("typeNum");
					Apps app = configService.getApp(typeNum);
					if (app == null) {
						continue;
					}
					AbstractModel<?, ?> model = JSONCover.toObject(jo, app
							.getModel());
					models.add(model.toRecord(host.getUser().getId()));
				}
			}
		}
		widgetService.batchUpdate(host.getUser().getId(), models);
	}
	
	private boolean joHasName(JSONObject jo, String name){
		try {
			return jo.has(name);
		} catch (Exception e) {
			return false;
		}
	}
	
}

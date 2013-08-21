package com.renren.fmwidget.services;

import java.io.InputStream;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;
import java.util.Map.Entry;

import net.sf.json.JSONObject;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.stereotype.Service;

import com.renren.fmwidget.enums.Apps;
import com.renren.fmwidget.model.AbstractModel;
import com.renren.fmwidget.utils.JSONCover;

@Service
public class WidgetDefaultConfigService implements InitializingBean {

	private Set<AbstractModel<?, ?>> config = new TreeSet<AbstractModel<?, ?>>();

	public Set<AbstractModel<?, ?>> getConfig() {
		return config;
	}

	public AbstractModel<?, ?> getConfig(Apps app) {
		for (AbstractModel<?, ?> model : config) {
			
			if (model.getClass() == app.getModel()) {
				return model;
			}
		}
		return null;
	}

	public Apps getApp(int appId) {
		for (Apps app : Apps.values()) {
			if (app.getId() == appId) {
				return app;
			}
		}
		return null;
	}

	@Override
	public void afterPropertiesSet() throws Exception {
		InputStream is = WidgetDefaultConfigService.class.getClassLoader()
				.getResourceAsStream("default-configration.properties");
		if (null == is) {
			return;
		}
		Properties p = new Properties();
		p.load(is);
		Set<Entry<Object, Object>> set = p.entrySet();
		for (Entry<Object, Object> entry : set) {
			JSONObject jo;
			try {
				jo = JSONObject.fromObject((String) entry.getValue());
			} catch (Exception e) {
				continue;
			}
			Class<? extends AbstractModel<?, ?>> clazz = null;
			for (Apps app : Apps.values()) {
				if (app.getName().equals((String) entry.getKey())) {
					clazz = app.getModel();
				}
			}
			if (null == clazz) {
				continue;
			}
			AbstractModel<?, ?> model = JSONCover.toObject(jo, clazz);
			if (null == model) {
				continue;
			}
			config.add(model);
		}
	}

}

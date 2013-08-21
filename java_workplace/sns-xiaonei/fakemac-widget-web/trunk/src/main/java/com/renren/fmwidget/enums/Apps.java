package com.renren.fmwidget.enums;

import com.renren.fmwidget.model.AbstractCustomDatas;
import com.renren.fmwidget.model.AbstractModel;
import com.renren.fmwidget.model.AbstractSystemDatas;
import com.renren.fmwidget.model.ClockModel;
import com.renren.fmwidget.model.StickyModel;
import com.renren.fmwidget.model.WeatherModel;

public enum Apps {

	memo(1, "sticky", StickyModel.class, StickyModel.StickySystemDatas.class,
			StickyModel.StickyCustomDatas.class),

	clock(2, "clock", ClockModel.class, ClockModel.ClockSystemDatas.class,
			ClockModel.ClockCustomDatas.class),

	weather(3, "weather", WeatherModel.class,
			WeatherModel.WeatherSystemDatas.class,
			WeatherModel.WeatherCustomDatas.class);

	private int id;

	private String name;

	private Class<? extends AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas>> model;

	private Class<? extends AbstractSystemDatas> systemDatas;

	private Class<? extends AbstractCustomDatas> customDatas;

	private Apps(
			int id,
			String name,
			Class<? extends AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas>> model,
			Class<? extends AbstractSystemDatas> systemDatas,
			Class<? extends AbstractCustomDatas> customDatas) {
		this.id = id;
		this.name = name;
		this.model = model;
		this.systemDatas = systemDatas;
		this.customDatas = customDatas;
	}

	public int getId() {
		return id;
	}

	public Class<? extends AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas>> getModel() {
		return model;
	}

	public String getName() {
		return name;
	}

	public Class<? extends AbstractSystemDatas> getSystemDatas() {
		return systemDatas;
	}

	public Class<? extends AbstractCustomDatas> getCustomDatas() {
		return customDatas;
	}
}

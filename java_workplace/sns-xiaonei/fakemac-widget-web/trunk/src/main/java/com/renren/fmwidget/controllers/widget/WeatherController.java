package com.renren.fmwidget.controllers.widget;

import java.io.IOException;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import com.renren.fmwidget.utils.WeatherUtils;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;

@Path("weather")
@LoginRequired
public class WeatherController {

	@Get("get")
	public String get(@Param("city") String city) {
		try {
			return "@" + WeatherUtils.getweather(city);
		} catch (Exception e) {
			e.printStackTrace();
			return "@{}";
		}
	}

	@Get("citys")
	public String citys() {
		try {
			return "@" + WeatherUtils.getCitys();
		} catch (IOException e) {
			e.printStackTrace();
			return "@{}";
		}
	}

}

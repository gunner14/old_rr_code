package com.renren.xoa.demo.controllers.window;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;

public class WeatherController {

	@Get
    public String xxx(Invocation inv) {
        return "@今天天气真好，花儿都开料!";
    }
	
}

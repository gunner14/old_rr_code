package com.xiaonei.reg.register.logic.additional;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.base.RegForm;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Mar 11, 2010 10:58:23 AM
 * 完成跳转到哪个地址的判断
 */
public class RegToUrlLogic {

    private static RegToUrlLogic _instance = new RegToUrlLogic();

    public static RegToUrlLogic getInstance() {
        return _instance;
    }

    private RegToUrlLogic() {
        super();
    }
    
    public RegForward getRetrunrURL(final RegMapping mapping,
            final RegForm regForm, final HttpServletRequest request,
            final HttpServletResponse response, boolean toHome){
        if(toHome){
            return new RegForward(Globals.urlHome.toString(), true);
        }
        //TODO
        
        //什么都不行的话，就直接去Home吧
        return new RegForward(Globals.urlHome.toString(), true);
    }
}

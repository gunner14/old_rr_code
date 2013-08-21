package com.xiaonei.commons.interceptors.access.newbiehelper;


import java.lang.annotation.Annotation;
import java.util.BitSet;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.newbie.service.NewbieHelperService;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.annotation.NewbieHelper;
import com.xiaonei.platform.core.model.User;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.sf.json.JSONObject;


/**
 * 获取到guide用户在训导系统中的相应数据。<br/>
 * 
 * @author ji.wei
 * 
 */
public class NewbieHelperInterceptor extends ControllerInterceptorAdapter {

	@Autowired
	private HostHolder hostHolder;
	
	
	public NewbieHelperInterceptor(){
		 setPriority(-2000);
	}
	
	
    @Override
    protected final List<Class<? extends Annotation>> getRequiredAnnotationClasses() {
        return createList(1).add(NewbieHelper.class).getList();
    }

    @Override
    protected BitSet getAnnotationScope(Class<? extends Annotation> annotationType) {
        if (NewbieHelper.class == annotationType) {
            return AnnotationScope.getMethodAndClassAndAnnotationScope();
        }
        return AnnotationScope.getMethodAndClassScope();
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        //
        User user = hostHolder.getUser();
        //未登录 或者 home 用户不显示
        if (user == null || user.getUserState().isGuideDone()) {
            return true;
        }
        
        JSONObject step = NewbieHelperService.getInstance().getStepJson(user.getId());
        if(step != null){
        	inv.addModel("newbieHelperStep", step.toString());
        }
        
        return true;
    }
}

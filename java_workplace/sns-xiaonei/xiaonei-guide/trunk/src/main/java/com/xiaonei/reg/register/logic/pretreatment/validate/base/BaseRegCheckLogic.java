/**
 *
 */
package com.xiaonei.reg.register.logic.pretreatment.validate.base;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;

/**
 * BaseRegCheckLogic
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-27 - 下午08:50:41
 */
public abstract class BaseRegCheckLogic implements IRegCheckCodeDef {

    public abstract String check(BaseRegForm regForm);

    protected static final RegLogger logger = RegLogger.getLogger("regLog",
            "BaseRegCheckLogic");
    
    private static final BaseRegFormLogic formLogic= new BaseRegFormLogic() ;
    
    protected BaseRegCheckLogic(){
    }

    final protected boolean asImplements(Object obj, Class<?> iface) {
    	return formLogic.asImplements(obj, iface);
    }

    final protected Object coverInterface(Object obj, Class<?> iface)
            throws Exception {
    	return formLogic.coverInterface(obj, iface);
    }

	public boolean isOk(String rt) {
		return OK.equals(rt);
	}
    
	public boolean isNotOk(String rt){
		return !isOk(rt);
	}
	
	/**
	 *  从regform里面取得用户注册时候，填写的accout
	 *
	 * @param form
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 4, 2009 - 11:45:13 PM
	 */
	public final static String getRegAccount(BaseRegForm form){
		return formLogic.getRegAccount(form);
	}
	
	/**
	 * getRegAccount 
	 *
	 *  从IAccType-里面取得用户注册时候，填写的accout
	 *  
	 * @param accType
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 4, 2009 - 11:53:00 PM
	 */
	public final static String getRegAccount(IAccType accType){
		return formLogic.getRegAccount(accType);
	}
}

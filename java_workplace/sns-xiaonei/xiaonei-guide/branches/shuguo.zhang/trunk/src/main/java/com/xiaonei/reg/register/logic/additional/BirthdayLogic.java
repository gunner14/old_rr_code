/**
 * 
 */
package com.xiaonei.reg.register.logic.additional;

import java.util.Calendar;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserBasicAdapter;

/**
 * @author wangtai
 * 
 */
public class BirthdayLogic {
	private static BirthdayLogic instance = new BirthdayLogic();

	private BirthdayLogic() {
		super();
	}

	public static BirthdayLogic getInstance() {
		return instance;
	}

	public boolean less17age(HttpServletRequest request) {
		User user = BaseThreadUtil.currentHost(request);
		return less17age(user);
	}

	public boolean less17age(User user) {
		// Inshion 2009-9-4 开心7号上线版，不能出现18岁以上的流程！
		// Inshion 2009-9-18 此处去掉，因为开心校内一样了，18岁以上就以上以上吧！
		// if("kaixin.com".equals(user.getDomain()) ||
		// "kaixin.com".equals(OpiConstants.domain.toString())){
		// return true;
		// }
		// 2009-10-21 16:26:45 修改，全部走17以下
		return true;
		// try {
		// int id = user.getId();
		// UserBasic ub = WUserBasicAdapter.getInstance().get(id);
		// return Calendar.getInstance().get(Calendar.YEAR)
		// - ub.getBirthYear() <= 17;
		// } catch (Exception e) {
		// e.printStackTrace();
		// return false;
		// }
	}

	public boolean compareYear(int id, int small, int big) {
		try {
			UserBasic ub = WUserBasicAdapter.getInstance().get(id);
			return ub.getBirthYear() >= small && ub.getBirthYear() <= big;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}

	public UserBasic getUserBasic(User host){
	    UserBasic ub = null;
	    try {
            ub = WUserBasicAdapter.getInstance().get(host.getId());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ub;
	}
	
	public static void main(String[] args) {

	}

	public int getAge(User host) {
		try {
			UserBasic ub = WUserBasicAdapter.getInstance().get(host.getId());
			return Calendar.getInstance().get(Calendar.YEAR)
					- ub.getBirthYear();
		} catch (Exception e) {
			e.printStackTrace();
			return 1960;
		}
	}

}

package com.xiaonei.reg.common.users;

import java.sql.SQLException;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.reg.register.dao.UserPassportExtDAO;
import com.xiaonei.reg.register.model.UserPassportExt;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.userlogincount.UserLoginCountAdapter;
import com.xiaonei.xce.username.UserNameInfo;
import com.xiaonei.xce.userurl.UserUrlInfo;

/**
 * UserInfo
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 9, 2009 - 11:37:45 AM
 * 
 */
public class UserInfo {

	private Integer id;
	private UserInfoBean userInfoBean;

	UserInfo(Integer id) {
		this.id = id;
		userInfoBean = new UserInfoBean(id);

	}

	UserInfo(User user) {
		this(user.getId());
	}

	/**
	 * getId
	 * 
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 9, 2009 - 12:11:15 PM
	 */
	public int getId() {
		return id;
	}

	/**
	 * getStage
	 * 
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 9, 2009 - 12:11:25 PM
	 */
	public int getStage() {
		return userInfoBean.getUserBasic().getStage();
	}

	/**
	 * 
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 9, 2009 - 12:11:34 PM
	 */
	public String getNewEmail() {
		UserPassportExt upe = new UserPassportExt();
		upe.setId(id);
		try {
			upe = UserPassportExtDAO.getInstance().select(upe);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return upe.getNew_email();

	}

	/**
	 * 
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 9, 2009 - 12:23:42 PM
	 */
	public String getEmailVerify() {
		UserPassportExt upe = userInfoBean.getUserPassportExt();
		if (null == upe) {
			return "";
		}
		return upe.getEmail_verify();
	}

	public int getSource() {
		return userInfoBean.getUserBasic().getStage();
	}

	public int getUniv() {
		return userInfoBean.getUserBasic().getUniv();
	}

	public String getHeadUrl() {
		return userInfoBean.getUserUrlInfo().getHeadUrl();
	}

	public void setStatus(int status) {
		userInfoBean.getUserBasic().setStatus(status);
	}

	public String getPassword() {
		return userInfoBean.getUserPassport().getPasswordMd5();
	}

	/**
	 * 
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 10, 2009 - 3:57:05 PM
	 */
	public String getVerifyCode() {
		UserPassportExt upe = userInfoBean.getUserPassportExt();
		if (null == upe) {
			return null;
		}
		return upe.getVerify_code();
	}

	public String getGender() {
		return userInfoBean.getUserBasic().getGender();
	}

	public String getHead_Url() {
		return HeadUrlUtil.getHeadFullUrl(userInfoBean.getUserUrlInfo()
				.getHeadUrl());
	}

	public String getLarge_Url() {
		return HeadUrlUtil.getHeadFullUrl(userInfoBean.getUserUrlInfo()
				.getLargeUrl());
	}

	public int getSelected() {
	    int state = userInfoBean.getUserState(id).getState();
	    String tmp = Integer.toBinaryString(state);
		return StringUtils.isEmpty(tmp) ? 0 : Integer.parseInt(tmp.substring(tmp.length()-1));
	}	

	public void incLoginCount() {
		try {
			userInfoBean.getUserLoginCountAdapter().getUserLoginCount(id);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void updateHeadUrl(String head, String main, String tiny,
			String large) {
		userInfoBean.getUserUrlInfo().setHeadUrl(head);
		userInfoBean.getUserUrlInfo().setLargeUrl(large);
		userInfoBean.getUserUrlInfo().setTinyUrl(tiny);
		userInfoBean.getUserUrlInfo().setMainUrl(main);
		UserAdapter.getUrlAdapter().setUserUrl(id,userInfoBean.getUserUrlInfo());
	}

	public void updateSelected(int selected) {
		//userInfoBean.getUserBasic().setSelected(selected);
		// TODO 2009-12-10 16:52:43 Selected没了
	}

	public String getTiny_Url() {
		return HeadUrlUtil.getHeadFullUrl(userInfoBean.getUserUrlInfo().getTinyUrl());
	}

	public String getName() {
		return userInfoBean.getUserNameInfo().getName();
	}

	public String getMain_Url() {
		return HeadUrlUtil.getHeadFullUrl(userInfoBean.getUserUrlInfo().getMainUrl());
	}

	public User getUser() {
		return userInfoBean.getUser();
	}

	public UserPassport getUserPassport() {
		return userInfoBean.getUserPassport();
	}

	public UserBasic getUserBasic() {
		return userInfoBean.getUserBasic();
	}

	public UserDesc getUserDesc() {
		return userInfoBean.getUserDesc();
	}

	public void setVerifyCode(int userId, String verifyCode) {
		userInfoBean.setVerifyCode(userId, verifyCode);
	}

	@SuppressWarnings("unused")
	private void setUserBasic(int id, UserBasic oceBasic) {
		//2010-01-20 拆分UserBasic
		//userInfoBean.setUserBasic(oceBasic.getId(), oceBasic);
	}

}

/**
 * UserInfoBean
 * 
 * 延迟加载小对象
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 10, 2009 - 7:02:44 PM
 * 
 */
class UserInfoBean {
	private int id;
	private User user;
	private UserNameInfo userNameInfo;
	private UserPassport userPassport;
	private UserLoginCountAdapter userLoginCountAdapter;
	private UserBasic userBasic;
	private UserDesc userDesc;
	private UserConfigInfo userConfigInfo;
	private UserUrlInfo userUrlInfo;
	private UserPassportExt userPassportExt;
	private UserState userState;
	
	public UserInfoBean(Integer id) {
		this.id = id;
	}

	@SuppressWarnings("unused")
	private void setUserBasic(int id, UserBasic oceBasic) {
		userBasic = null;
		//2010-01-20 拆分UserBasic
		//SnsAdapterFactory.getUserBasicAdapter().setUserBasic(oceBasic.getId(), oceBasic);
	}

	public UserPassportExt getUserPassportExt() {
		if (null == userPassportExt) {
			UserPassportExt upe = new UserPassportExt();
			upe.setId(id);
			try {
				userPassportExt = UserPassportExtDAO.getInstance().select(upe);
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return userPassportExt;
	}

	public void setVerifyCode(int userId, String verifyCode) {
		userPassportExt = new UserPassportExt();
		userPassportExt.setId(userId);
		userPassportExt.setVerify_code(verifyCode);
		try {
			UserPassportExtDAO.getInstance().updateVerifyCodeById(userPassportExt);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	public UserDesc getUserDesc() {
		if (null == userDesc) {
			userDesc = WUserDescAdapter.getInstance().get(id);
		}
		return userDesc;
	}

	public User getUser() {
		if (null == user) {
			try {
				user = WUserAdapter.getInstance().get(id);
			} catch (Exception e) {
				e.printStackTrace();
				return user;
			}
		}
		return user;
	}

	public UserPassport getUserPassport() {
		if (null == userPassport) {
			try {
				userPassport = WUserAdapter.getInstance().getPassportAdapter().getUserPassport(id);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return userPassport;
	}

	public UserLoginCountAdapter getUserLoginCountAdapter() {
		if (null == userLoginCountAdapter) {
			userLoginCountAdapter = WUserAdapter.getInstance().getLoginCountAdapter();
		}
		return userLoginCountAdapter;
	}

	public UserBasic getUserBasic() {
		if (null == userBasic) {
			userBasic = SnsAdapterFactory.getUserBasicAdapter().get(id);
		}
		return userBasic;
	}

	public UserConfigInfo getUserConfigInfo() {
		if (null == userConfigInfo) {
			userConfigInfo = WUserConfigInfoAdapter.getInstance().getUserConfig(id);
		}
		return userConfigInfo;
	}

	public UserUrlInfo getUserUrlInfo() {
		if (null == userUrlInfo) {
			userUrlInfo = UserAdapter.getUrlAdapter().getUserUrl(id);
		}
		return userUrlInfo;
	}

	public UserNameInfo getUserNameInfo() {
		if (null == userNameInfo) {
			try {
				this.userNameInfo = WUserAdapter.getInstance().getNameAdapter().getUserName(id);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return this.userNameInfo;
	}

    
    public UserState getUserState(int userId) {
        if (null == userState) {
            try {
                this.userState = WUserAdapter.getInstance().getStateAdapter().getUserState(userId);                        
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return this.userState;
    }

    
    public void setUserState(UserState userState) {
        this.userState = userState;
    }
}
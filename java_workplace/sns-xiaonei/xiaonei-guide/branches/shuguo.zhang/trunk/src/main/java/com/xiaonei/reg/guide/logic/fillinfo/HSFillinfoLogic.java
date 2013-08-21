package com.xiaonei.reg.guide.logic.fillinfo;

import java.sql.SQLException;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;

import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.jdbc.dao.HighSchoolDao;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.dao.ElementarySchoolDAO;
import com.xiaonei.reg.guide.model.ElementarySchool;
import com.xiaonei.reg.guide.pojo.HSFillInfoElementary;
import com.xiaonei.reg.guide.pojo.HSFillInfoJuniorPojo;
import com.xiaonei.reg.guide.pojo.HSFillInfoUpdatePojo;
import com.xiaonei.reg.guide.pojo.HSFillinfoPojo;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.register.dao.RegAuditDAO;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;

/**
 * @author dream
 * @date 2008-10-15
 */
public class HSFillinfoLogic {

    static class SingletonHolder {
        static HSFillinfoLogic instance = new HSFillinfoLogic();
    }

    public static HSFillinfoLogic getInstance() {
        return SingletonHolder.instance;
    }

    /**
     * 读取高中
     *
     * @param host
     * @return
     */
    public HighSchool getHighSchool(User host) {
        try {
            return HighSchoolDao.getInstance().get(UserAdapter.get(host).getUniv());
        } catch (SQLException e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    /**
     * 根据高中名读取高中
     *
     * @param juniorHighSchoolName
     * @return
     */
    public HighSchool getHighSchoolByName(String juniorHighSchoolName) {
        try {
            return HighSchoolDao.getInstance().getByName(juniorHighSchoolName);
        } catch (SQLException e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    /**
     * 根据小学名称读取小学
     *
     * @param elementarySchoolName
     * @return
     */
    public ElementarySchool getElementarySchoolByName(
            String elementarySchoolName) {
        try {
            return ElementarySchoolDAO.getInstance().getByName(
                    elementarySchoolName);
        } catch (SQLException e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    /**
     * 根据int型的schooltype判断跳转页面
     */
    public String checkSchoolType(int schoolType, User host) {
        if (schoolType == 0) {
            getHighSchool(host);
            return "succ";
        } else if (schoolType == 1) {
            return "cz";
        } else if (schoolType == 2) {
            return "jx";
        }
        return "succ";
    }

    public int getSchoolType(User host) {
        try {
            return RegAuditDAO.getInstance().getSchoolType(host.getId());
        } catch (SQLException e) {
            e.printStackTrace(System.err);
        }
        return 0;
    }

    public boolean checkHighSchoolInfo(HSFillinfoPojo h) {
        return Utils.getInstance().validateForString(h.getHomeProvince())
                || Utils.getInstance().validateForString(h.getHomeCityName())
                || Utils.getInstance().validateForString(h.getHomeCityCode()) ? true
                : false;
    }

    public boolean checkJuniorHighSchoolInfo(String juniorHighSchoolName) {
        return Utils.getInstance().validateForString(juniorHighSchoolName) ? true
                : false;
    }

    public boolean checkSchoolType(int schoolType) {
        return (schoolType == 0 || schoolType == 2) ? true : false;
    }

    /**
     * 更新高中入学信息
     *
     * @param h
     */
	public void setHighSchoolData(HSFillInfoUpdatePojo h) {
        try {
        	//2010-01-20 拆分UserBasic
            /*UserBasic oceBasic = new UserBasic();
            oceBasic.setId(h.getHostid());
            oceBasic.setHomeProvince(h.getHomeProvince());
            oceBasic.setHomeCity(h.getHomeCityName());
            oceBasic.setCityCode(h.getHomeCityCode());
            UserAdapter.setBasic(oceBasic);*/
            
            UserBorn ub = new UserBorn();//SnsAdapterFactory.getUserBornAdapter().getUserBorn(h.getHostid());
            ub.setHomeProvince(h.getHomeProvince());
            ub.setHomeCity(h.getHomeCityName());
            ub.setCityCode(h.getHomeCityCode());
    		SnsAdapterFactory.getUserBornAdapter().setUserBorn(h.getHostid(), ub);
    		
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }

    }

    /**
     * 更新初中入学信息
     *
     * @param h
     */
    public void updateJuniorSchoolInfo(HSFillInfoJuniorPojo h) {
        try {
            JuniorHighSchoolInfo jhsInfo = AdapterFactory.getNetworkAdapter()
                    .getJuniorHighSchoolInfo(h.getHost().getId());
            if (jhsInfo == null) {
                jhsInfo = new JuniorHighSchoolInfo();
                jhsInfo.setJunHighentarySchoolId(h.getJuniorHighSchoolId());
                jhsInfo.setJunHighentarySchoolName(h.getJuniorHighSchoolName());
                jhsInfo.setJunHighentarySchoolYear(h.getJuniorHighSchoolYear());
                jhsInfo.setUserId(h.getHost().getId());
                AdapterFactory.getNetworkAdapter().addJuniorHighSchoolInfo(
                        h.getHost().getId(), jhsInfo);
            } else {
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }

    }

    /**
     * 更新小学信息
     *
     * @param h
     */
    public void updateElementarySchoolInfo(HSFillInfoElementary h) {
        try {
            ElementarySchoolInfo esInfo = AdapterFactory.getNetworkAdapter()
                    .getElementarySchoolInfo(h.getUser().getId());
            if (esInfo == null) {
                esInfo = new ElementarySchoolInfo();
                esInfo.setElementarySchoolId(h.getElementarySchoolId());
                esInfo.setElementarySchoolName(h.getElementarySchoolName());
                esInfo.setElementarySchoolYear(h.getElementarySchoolYear());
                esInfo.setUserId(h.getUser().getId());
                AdapterFactory.getNetworkAdapter().addElementarySchoolInfo(
                        h.getUser().getId(), esInfo);
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

	}
}

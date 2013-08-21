/**
 *
 */
package com.xiaonei.reg.register.logic.impl;

import java.sql.SQLException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.RegAuditHighSchoolDAO;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegAuditLogic;
import com.xiaonei.reg.register.model.RegAppAuditHighSchool;
import com.xiaonei.reg.register.model.RegAuditHighSchool;
import com.xiaonei.reg.register.pojo.AuditBasePojo;
import com.xiaonei.reg.register.pojo.AuditHSPojo;

/**
 * 高中审核注册
 * <p/>
 * RegAuditHsLogicImpl.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-9 下午04:41:52
 */
public class RegAuditHsLogicImpl implements IRegAuditLogic {
    private static RegAuditHsLogicImpl _instance = new RegAuditHsLogicImpl();

    public static RegAuditHsLogicImpl getInstance() {
        return _instance;
    }

    // 高中审核注册
    public int audit(AuditBasePojo regb) {
        AuditHSPojo reg = null;
        if (regb instanceof AuditHSPojo) {
            reg = (AuditHSPojo) regb;
        } else {
            return err_system;
        }
        // 高中审核
        // 需审核的信息: 入学年、班级、学号、头像
        int enrollYear = 0;
        try {
            enrollYear = reg.getEnrollYear();
        } catch (Exception e) {
        }
        if (enrollYear == 0) {
            // return mapping.findForward(regAudit);
            return err_no_enrollyear;
        }

        String highSchoolClass = Escape.trim(reg.getHighSchoolClass());
        if (highSchoolClass == null || highSchoolClass.length() == 0) {
            // return mapping.findForward(regAudit);
            return err_no_class;
        }

        if (regb.getLargeUrl() == null || "".equals(regb.getLargeUrl()) || regb.getMainUrl() == null || "".equals(regb.getMainUrl())
                || regb.getTinyUrl() == null || "".equals(regb.getTinyUrl()) || regb.getHeadUrl() == null || "".equals(regb.getHeadUrl())) {
            return err_no_portrait;
        } else {
            // 验证pic是否合法,防止XSS
            if (!validatePic(regb.getLargeUrl()) || !validatePic(regb.getMainUrl()) || !validatePic(regb.getTinyUrl()) || !validatePic(regb.getHeadUrl()))
                return err_no_portrait;
        }
        int returnId = 0;
        try {
            /* 如果是邀请注册需要传入库中的数据 */
            String strAppId = reg.getAppId();
            String appToken = reg.getAppToken();
            String fromType = reg.getFromType();
            String strFromId = reg.getInviterId();

            RegAuditHighSchool rah = new RegAuditHighSchool();
            rah.setEmail(reg.getEmail());
            rah.setPassword(reg.getPwd());
            rah.setName(reg.getName());
            rah.setSchoolType(reg.getSchoolType());
            rah.setHighSchoolCode(reg.getHighSchoolCode());
            rah.setHighSchoolName(reg.getHighSchoolName());
            rah.setStudentNumber("");
            rah.setEnrollYear(enrollYear);
            rah.setHighSchoolClass(highSchoolClass);

            rah.setLargeurl(reg.getLargeUrl());
            rah.setMainurl(reg.getMainUrl());
            rah.setHeadurl(reg.getHeadUrl());
            rah.setTinyurl(reg.getTinyUrl());

            rah.setIp(reg.getIp());
            rah.setGender(reg.getGender());
            // 分组相关
            rah.setInviterId(Integer.parseInt(strFromId));
            rah.setGroupName(reg.getGroupName());

            int id = RegAuditHighSchoolDAO.getInstance().save(rah);
            rah.setId(id);
            returnId = -id;

            if ((null != strAppId && !"".equals(strAppId) && !"0".equals(strAppId))) {
                int appId = (Integer.parseInt(strAppId));
                User fromUser = UserAdapter.getUser(Integer.parseInt(strFromId));
                RegAppAuditHighSchool raa = new RegAppAuditHighSchool();
                raa.setId(id);
                raa.setApp_id(appId);
                raa.setApp_token(appToken);
                raa.setFrom_id(fromUser.getId());
                raa.setFrom_name(fromUser.getName());
                raa.setInvite_type(fromType);
                RegAuditHighSchoolDAO.getInstance().insertRegAppAuditHS(raa);
            }
        } catch (RuntimeException e) {
            e.printStackTrace(System.err);
            return err_system;
        } catch (SQLException e) {
			e.printStackTrace();
		}catch (Exception e) {
			e.printStackTrace();
		}
        return returnId;
    }

    public boolean validatePic(String pic) {
        char c = 0;
        for (int i = 0; i < pic.length(); i++) {
            c = pic.charAt(i);
            if (!(c >= 65 && c <= 90) && !(c >= 97 && c <= 122) && c != 95 && !(c >= 46 && c <= 57)) {
                return false;
            }
        }
        return true;
    }

    public static void main(String[] args) {


    }

    /*
      * (non-Javadoc)
      *
      * @see
      * com.xiaonei.reg.register.logic.RegAuditLogic#shouldBeAudit(com.xiaonei
      * .reg.register.form.RegForm, java.lang.String)
      */
    public int shouldBeAudit(BaseRegForm regForm, String ip) {
        // 必须审核
        return isAudit;
    }

}

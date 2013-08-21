package com.xiaonei.reg.guide.logic.fillinfo;

import java.sql.SQLException;

import com.dodoyo.datum.dao.DepartmentDAO;
import com.dodoyo.datum.model.Department;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.jdbc.dao.UniversityDAO;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.dao.DeptAddDAO;
import com.xiaonei.reg.guide.model.DeptAdd;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.tribe.dao.TribeDAO;
import com.xiaonei.tribe.dao.TribeUserDAO;
import com.xiaonei.tribe.model.Tribe;
import com.xiaonei.tribe.model.TribeUser;

public class CommonFillinfoLogic {

    private static CommonFillinfoLogic instance = new CommonFillinfoLogic();

    public static CommonFillinfoLogic getInstance() {
        return instance;
    }

    public University getUniv(User host) {
        try {
            return UniversityDAO.getInstance().get(UserAdapter.get(host).getUniv());
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public UserBasic getUserBasic(User host) {
        try {
            return SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    public static void collectDept(int userId, int univId, String dept) {
        if (dept == null || dept.length() == 0) {
            return;
        }
        try {
            if (DepartmentDAO.getInstance().getByDept(dept, univId) == null) {
                if (new DeptAddDAO().getByDept(dept, univId) == 100) {
                    Department d = new Department();
                    d.setUniv(univId);
                    d.setName(dept);
                    d.setStuCount(100);
                    DepartmentDAO.getInstance().save(d);
                }

                DeptAdd da = new DeptAddDAO().getByUser(userId);
                if (da == null) {
                    da = new DeptAdd(univId, userId, dept);
                    new DeptAddDAO().save(da);
                } else {
                    da.setDept(dept);
                    new DeptAddDAO().update(da);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }


    }

    public static void joinGroup(int userId, int groupId) {
        if (groupId == 0) {
            return;
        }
        try {
            Tribe tr = TribeDAO.getInstance().get(groupId);
            if (tr != null) {
                TribeUser tu = TribeUserDAO.getInstance().get(groupId, userId);
                if (tu == null) {
                    com.xiaonei.tribe.home.TribeHome.addTribeMember(userId, groupId);
                    TribeDAO.getInstance().incMemberCount(tr.getId());
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }
}

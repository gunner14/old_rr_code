package com.xiaonei.platform.core.opt.adminPermission;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.xce.storm.XceStormAdapter;
import com.xiaonei.xce.storm.XceStormListener;

public class AdminManager {

    public final static String UPDATEFLAG = "admin.right.update";

    private static Log logger = LogFactory.getLog(AdminManager.class);

    private static AdminManager instance = new AdminManager();

    private Map<Integer, AdminUser> rightMap = new HashMap<Integer, AdminUser>();

    private AdminManager() {
        XceStormListener listener = new XceStormListenerImpl();
        /**** 实现监听方法类 ***/

        XceStormAdapter.getInstance().subscribe(UPDATEFLAG, listener);
        /*** 注册监听方法 ***/

        this.loadRight();
    }

    public static AdminManager getInstance() {
        return instance;
    }

    public void loadRight() {
        List<AdminUser> admins = this.getAdminUserList();
        if (admins != null) {
            for (int i = 0; i < admins.size(); i++) {
                AdminUser admin = admins.get(i);
                loadRight(admin);
            }
        }
    }

    public void loadRight(int userId) {
        AdminUser admin = this.getAdminUser(userId);
        if (admin != null) {
            loadRight(admin);
        }
    }

    private void loadRight(AdminUser admin) {
        admin.setRight(this.getRightByUserId(admin.getId()));
        rightMap.put(admin.getId(), admin);
    }

    public void reloadRight() {
        rightMap.clear();
        this.loadRight();
    }

    public boolean isAdmin(int userId) {
        return rightMap.containsKey(userId);
    }

    public boolean isSuperAdmin(int userId) {
        if (isAdmin(userId) && rightMap.get(userId).getAuth() == User.authSuperAdmin) {
            return true;
        } else {
            return false;
        }
    }

    public boolean hasRight(int userId, int functionId, int right) {
        if (isSuperAdmin(userId)) {//判断是否是超级管理员,如果是就通过
            return true;
        }
        if (!isAdmin(userId)) {//判断是否是管理员
            return false;
        }
        List<int[]> rights = rightMap.get(userId).getRight();
        if (rights == null) {//判断管理员的权限是否为空
            return false;
        }
        for (int i = 0; i < rights.size(); i++) {
            int[] rright = rights.get(i);
            if (rright[0] == functionId) {
                if ((rright[1] & right) == right) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        return false;
    }

    public AdminUser getAdminUser(int userId) {
        Connection con = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        AdminUser admin = null;
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_USER_COMMON, false);
            String sql = "select id,auth from user_admin where id = ?";
            ps = con.prepareStatement(sql);
            ps.setInt(1, userId);
            rs = ps.executeQuery();
            if (rs.next()) {
                admin = new AdminUser();
                admin.setId(rs.getInt("id"));
                admin.setAuth(rs.getInt("auth"));
            }
            return admin;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                rs.close();
                ps.close();
            } catch (Exception ex) {

            }
            DataAccessMgr.getInstance().closeConnection(con);
        }
    }

    public List<AdminUser> getAdminUserList() {
        Connection con = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<AdminUser> list = null;
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_USER_COMMON, false);
            String sql = "select id,auth from user_admin";
            ps = con.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()) {
                if (list == null) {
                    list = new ArrayList<AdminUser>();
                }
                AdminUser admin = new AdminUser();
                admin.setId(rs.getInt("id"));
                admin.setAuth(rs.getInt("auth"));
                list.add(admin);
            }
            return list;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                rs.close();
                ps.close();
            } catch (Exception ex) {

            }
            DataAccessMgr.getInstance().closeConnection(con);
        }
    }

    public List<int[]> getRightByUserId(int userId) {
        Connection con = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<int[]> list = null;
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_COMMON, false);
            String sql = "select function_id,rright from admin_right where userid = ?";
            ps = con.prepareStatement(sql);
            ps.setInt(1, userId);
            rs = ps.executeQuery();
            while (rs.next()) {
                if (list == null) {
                    list = new ArrayList<int[]>();
                }
                int[] right = { rs.getInt("function_id"), rs.getInt("rright") };
                list.add(right);
            }
            return list;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                rs.close();
                ps.close();
            } catch (Exception ex) {

            }
            DataAccessMgr.getInstance().closeConnection(con);
        }
    }

    class XceStormListenerImpl extends XceStormListener {

        private static final long serialVersionUID = 1L;

        @Override
        public void handle(Map<String, String> context) {
            final String param = context.get(UPDATEFLAG);
            if (logger.isDebugEnabled()) {
                logger.debug("accept xceStormMsg: " + param);
            }
            
            //随机一个延迟执行reload操作，防止所有机器同时reload对DB造成的压力
            Random ran = new Random();
            final int delay = ran.nextInt(200);
            new Thread(new Runnable() {

				@Override
				public void run() {
					try {
						Thread.sleep(delay * 100);	//希望的最小间隔是100ms
					} catch (InterruptedException e1) {
						e1.printStackTrace();
					}
					if (null != param) {
						int userId = 0;
						try {
							userId = Integer.parseInt(param);
							if (userId <= 0) {// 刷新所有权限
								reloadRight();
							} else {// 刷新指定用户的权限
								loadRight(userId);
							}
						} catch (Exception e) {
							logger.error(e.getMessage(), e);
						}

					}
				}
			}).start();
        }
    }

}

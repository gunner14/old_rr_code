package com.xiaonei.platform.component.space.home;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.xiaonei.platform.component.space.model.PermissionBatchModel;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.adminPermission.AdminManager;
import com.xiaonei.platform.core.opt.permission.License;
import com.xiaonei.platform.core.opt.permission.PermissionFactory;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.define.Module;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.impl.RelationExpImpl;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;
import com.xiaonei.platform.core.opt.permission.strategy.impl.ProfilePrivacy;

/**
 * 
 * Space服务对外提供的Home接口类
 */
public class SpaceHome {

    private static SpaceHome instance = new SpaceHome();

    public static SpaceHome getInstance() {
        return instance;
    }

    private SpaceHome() {
        super();
    }

    /**
     * 得到一个人的高中资料列表
     * 
     * @param userId
     * @return
     */
    public List<HighSchoolInfo> getHighschoolInfoList(int userId) {
        try {
            return AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(userId);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    /**
     * 得到一个人的大学资料列表
     * 
     * @param userId
     * @return
     */
    public List<UniversityInfo> getUniversityInfoList(int userId) {
        try {
            return AdapterFactory.getNetworkAdapter().getUniversityInfoList(userId);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 判断用户是否有权看对方基本信息,返回boolean
     * 
     * @param visitor 访问者，访问主页的游客，当前登录者
     * @param owner 拥有者，主页的所有者，被浏览者
     * @param ownerUci UserConfigInfo
     * @return boolean
     * 
     * @author tai.wang@opi-corp.com Jul 15, 2010 - 7:45:43 PM
     */
    public boolean getVisitBasicInfoPermission(User visitorUser, User ownerUser,
            UserConfigInfo ownerUci) {
        ProfilePrivacy pp = getProfilePrivacy(visitorUser, ownerUser, ownerUci);
        return pp.getShowBasicInfo();
    }

    /**
     * getVisitPermissionBatch<br>
     * 
     * @param vistor {@link Status} 访问者
     * @param targetModels {@link PermissionBatchModel} 被访问的人群
     * @return Map<Integer, ProfilePrivacy> integer 表示受体id,ProfilePrivacy
     *         表示是否权限集合
     * @throws NullPointerException 参数为null
     * 
     * @author tai.wang@opi-corp.com Aug 4, 2010 - 3:04:25 PM
     */
    public Map<Integer, ProfilePrivacy> getVisitPermissionBatch(Status vistor,
            PermissionBatchModel[] targetModels) throws NullPointerException {
        if (null == vistor || targetModels == null) {
            throw new NullPointerException();
        }
        Map<Integer, ProfilePrivacy> permissionMap = new HashMap<Integer, ProfilePrivacy>(
                targetModels.length);
        for (PermissionBatchModel permissionBatchModel : targetModels) {
            License license;
            try {
                license = getVisitPermission(vistor, permissionBatchModel.getTarget());
            } catch (Exception e) {
                e.printStackTrace();
                license = null;
            }
            if (null == license) {
                continue;
            }
            int path = license.getPath();
            ProfilePrivacy pp = new ProfilePrivacy(path,permissionBatchModel.getTargetProfilePrivacy() ,vistor.getAuth());
            if (pp != null) {
                pp.setVisitor(vistor.getUserId());
                permissionMap.put(permissionBatchModel.getTargetId(), pp);
            } else {
                continue;
            }
        }
        return permissionMap;
    }

    public static void main(String[] args){
//    	User self = SnsAdapterFactory.getUserAdapter().get(68126);
//    	User owner = SnsAdapterFactory.getUserAdapter().get(265212007);
//    	int sourceControl = SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(265212007).getBasicConfig();
//    	License license = null;// 许可对象，是否可以访问主页的许可
//    	license = SpaceHome.getInstance().getVisitPermission(self, owner, sourceControl);
    }
    /**
     * 判断用户是否有权看一个学校,返回boolean
     * 
     * @param visitor 访问者，访问主页的游客，当前登录者
     * @param owner 拥有者，主页的所有者，被浏览者
     * @param ownerUci UserConfigInfo
     * @return boolean
     * 
     * 
     * @author tai.wang@opi-corp.com Jul 15, 2010 - 7:42:46 PM
     */
    public boolean getVisitEducationPermission(User visitorUser, User ownerUser,
            UserConfigInfo ownerUci) {
        ProfilePrivacy pp = getProfilePrivacy(visitorUser, ownerUser, ownerUci);
        return pp.getShowEducation();
    }

    /**
     * 判断用户是否有权看一个人的留言列表，将返回一个License对象
     * 
     * @param visitorUser 访问者，当前登录者
     * @param ownerUser 拥有者，主页的所有者，被浏览者
     * @param control 隐私设置值
     * @return 返回一个SpacePermission对象，内部包含有详细信息
     */
    public License getVisitGossipPermission(User visitorUser, User ownerUser, int control) {
        try {
            int path = 0;
            //访问者
            Status vistor = null;
            if (visitorUser != null) {
                vistor = new Status(visitorUser.getId(), visitorUser.getId(), visitorUser
                        .getStage(), visitorUser.getUniv(), visitorUser.getAuth(), visitorUser
                        .getStatus(), 0);
            } else {
                vistor = new Status(0, 0, 0, 0, 0, 0, 0);
            }
            // -----------------权限控制 开始-----------------
            Status owner = new Status(ownerUser.getId(), ownerUser.getId(), ownerUser.getStage(),
                    ownerUser.getUniv(), ownerUser.getAuth(), ownerUser.getStatus(), control);
            path = RelationExpImpl.getInstance().getRelationPath(vistor, owner);
            vistor.setFromToPath(path);
            Ret r = getSelectPermission(vistor, owner);
            //			path = vistor.getFromToPath();

            if (r != null && !r.hasRight()
                    && !AdminManager.getInstance().hasRight(visitorUser.getId(), 162, 1)) {
                //无权限看
                return new License(false, path, r.getMsg());
            } else {
                //允许看
                return new License(true, path);
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
            //无权限看
            return new License(false, RelationDef.uuFriend, null);
        }
    }

    /**
     * 判断用户是否有权看一个主页，将返回一个SpacePermission对象
     * 
     * @param visitor 访问者，访问主页的游客，当前登录者
     * @param owner 拥有者，主页的所有者，被浏览者
     * @param control 隐私设置值
     * @return 返回一个SpacePermission对象，内部包含有详细信息
     */
    public License getVisitPermission(User visitorUser, User ownerUser, int control) {
        try {
            //访问者
            Status vistor = null;
            if (visitorUser != null) {
                vistor = new Status(visitorUser.getId(), visitorUser.getId(), visitorUser
                        .getStage(), visitorUser.getUniv(), visitorUser.getAuth(), visitorUser
                        .getStatus(), 0);
            } else {
                vistor = new Status(0, 0, 0, 0, 0, 0, 0);
            }
            // -----------------权限控制 开始-----------------
            Status owner = new Status(ownerUser.getId(), ownerUser.getId(), ownerUser.getStage(),
                    ownerUser.getUniv(), ownerUser.getAuth(), ownerUser.getStatus(), control);
            return getVisitPermission(vistor, owner);
        } catch (Exception e) {
            e.printStackTrace(System.err);
            //无权限看主页
            return new License(false, RelationDef.uuFriend, null);

        }
    }

    /**
     * getVisitPermission<br>
     * 
     * @param vistor {@link Status}
     * @param owner {@link Status}
     * @return {@link License}
     * 
     * @author tai.wang@opi-corp.com Aug 4, 2010 - 2:56:38 PM
     */
    private License getVisitPermission(Status vistor, Status owner) {
        int path = 0;
        path = RelationExpImpl.getInstance().getRelationPath(vistor, owner);
        vistor.setFromToPath(path);
        Ret r = getSelectPermission(vistor, owner);
        path = vistor.getFromToPath();
        if (!r.hasRight() && !AdminManager.getInstance().hasRight(vistor.getUserId(), 162, 2)) {
            //无权限看主页
            return new License(false, path, r.getMsg());
        } else {
            //允许看主页
            return new License(true, path);
        }
    }

    /**
     * 判断用户是否有权看一个公司,返回boolean
     * 
     * @param visitor 访问者，访问主页的游客，当前登录者
     * @param owner 拥有者，主页的所有者，被浏览者
     * @param ownerUci UserConfigInfo
     * @return boolean
     * 
     * @author tai.wang@opi-corp.com Jul 15, 2010 - 7:12:22 PM
     */
    public boolean getVisitWorkPermission(User visitorUser, User ownerUser, UserConfigInfo ownerUci) {
        ProfilePrivacy pp = getProfilePrivacy(visitorUser, ownerUser, ownerUci);
        return pp.getShowWork();
    }

    /**
     * 得到一个人的工作信息列表
     * 
     * @param userId
     * @return
     */
    public List<WorkplaceInfo> getWorkplaceInfoList(int userId) {
        try {
            return AdapterFactory.getNetworkAdapter().getWorkplaceInfoList(userId);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * getProfilePrivacy<br>
     * 获得profile 上的隐私
     * 
     * @param visitorUser
     * @param ownerUser
     * @param ownerUci
     * @return
     * 
     * @author tai.wang@opi-corp.com Jul 15, 2010 - 7:41:50 PM
     */
    private ProfilePrivacy getProfilePrivacy(User visitorUser, User ownerUser,
            UserConfigInfo ownerUci) {
        ProfilePrivacy pp = null;

        if (ownerUci != null) {
            int authValue = visitorUser.getAuth();
            int path;
            try {
                path = this.getVisitPermission(visitorUser, ownerUser, ownerUci.getBasicConfig())
                        .getPath();
            } catch (Exception e) {
                e.printStackTrace();
                path = RelationDef.uuNoPath;
            }
            if ((ownerUci.getProfilePrivacy() == null)
                    || (ownerUci.getProfilePrivacy().trim().length() == 0)) {
                pp = new ProfilePrivacy(path, authValue);
            } else {
                pp = new ProfilePrivacy(path, ownerUci.getProfilePrivacy(), authValue);
            }
        }
        if (pp != null) pp.setVisitor(visitorUser.getId());
        return pp;
    }

    private Ret getSelectPermission(Status guester, Status source) {
        PermissionFactory pfc = PermissionFactory.getInstance();
        try {
            return pfc.getPermissionByModule(Module.MODULE_SPACE).hasSelect(guester, source);
        } catch (Exception e) {
            e.printStackTrace(System.err);
            return Strategy.RET_TRUE;
        }
    }

}

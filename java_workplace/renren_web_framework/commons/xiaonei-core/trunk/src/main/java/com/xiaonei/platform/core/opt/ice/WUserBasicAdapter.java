package com.xiaonei.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.factory.WUserBasicFactory;
import com.xiaonei.xce.userbasic.UserBasicAdapter;
import com.xiaonei.xce.userborn.UserBornAdapter;
import com.xiaonei.xce.userborn.UserBornFactory;
import com.xiaonei.xce.userborn.UserBornInfo;
import com.xiaonei.xce.userdoing.UserDoingAdapter;
import com.xiaonei.xce.userdoing.UserDoingFactory;
import com.xiaonei.xce.userdoing.UserDoingInfo;
import com.xiaonei.xce.userstage.UserStageAdapter;
import com.xiaonei.xce.userstage.UserStageFactory;
import com.xiaonei.xce.userstage.UserStageInfo;

public class WUserBasicAdapter {

    private UserBornAdapter<UserBornInfo> bornAdapter;

    private UserDoingAdapter<UserDoingInfo> doingAdapter;

    private UserStageAdapter<UserStageInfo> stageAdapter;

    private UserBasicAdapter<UserBasic> oceAdapter;

    private volatile static WUserBasicAdapter instance = null;

    public static WUserBasicAdapter getInstance() {
        if (instance == null) {
            synchronized (WUserBasicAdapter.class) {
                if (instance == null) {
                    instance = new WUserBasicAdapter();
                }
            }
        }
        return instance;
    }

    private WUserBasicAdapter() {
        oceAdapter = new UserBasicAdapter<UserBasic>(new WUserBasicFactory());

        bornAdapter = new UserBornAdapter<UserBornInfo>(new UserBornFactory<UserBornInfo>() {

            @Override
            public UserBornInfo create() {
                return new UserBornInfo();
            }
        });
        doingAdapter = new UserDoingAdapter<UserDoingInfo>(new UserDoingFactory<UserDoingInfo>() {

            @Override
            public UserDoingInfo create() {
                return new UserDoingInfo();
            }
        });
        stageAdapter = new UserStageAdapter<UserStageInfo>(new UserStageFactory<UserStageInfo>() {

            @Override
            public UserStageInfo create() {
                return new UserStageInfo();
            }
        });
    }

    public UserBasic get(int id) {
        if (id <= 0) return null;
        try {
            return oceAdapter.getUserBasic(id);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

//    public void setUserBasic(int userid, UserBasic ubi) {
//        oceAdapter.setUserBasic(userid, ubi);
//    }
//
//    public void createUserBasic(int userId, UserBasic ubi) {
//        oceAdapter.createUserBasic(userId, ubi);
//    }

    //-----------------

    public UserBornAdapter<UserBornInfo> getBornAdapter() {
        return bornAdapter;
    }

    public UserDoingAdapter<UserDoingInfo> getDoingAdapter() {
        return doingAdapter;
    }

    public UserStageAdapter<UserStageInfo> getStageAdapter() {
        return stageAdapter;
    }

}

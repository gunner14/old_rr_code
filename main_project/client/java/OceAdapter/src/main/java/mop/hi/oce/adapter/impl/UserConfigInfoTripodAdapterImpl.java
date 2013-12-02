package mop.hi.oce.adapter.impl;

import mop.hi.oce.domain.user.UserConfigInfo;
import mop.hi.oce.domain.user.UserConfigInfoFactory;
import mop.hi.svc.model.UserException;
import xce.tripod.TripodClient;
import xce.tripod.UserConfig.UserConfigData;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.log.Oce;

public class UserConfigInfoTripodAdapterImpl {

    public static final String NAME_SPACE = "xce_user";

    public static final String BIZ_ID = "user_config";

    private static final int EXPIRE_TIME = 30 * 24 * 3600;

    private UserConfigInfoFactory mFactory = null;

    private TripodClient mTripodClient = null;

    public UserConfigInfoTripodAdapterImpl(UserConfigInfoFactory factory) {
        mFactory = factory;
        mTripodClient = new TripodClient(NAME_SPACE, BIZ_ID);
    }

    // 抛出两个异常
    // 1. Ice.TimeoutException      从CacheManager取数据超时
    // 2. Ice.UnknownUserException  DB中无用户数据
    // 其他异常情况直接返回默认构造的UserConfigInfo结构
    public UserConfigInfo getUserConfig(int userId) throws UserException, Exception {
        String callInfo = this.getClass().getName() + ".getUserConfig  id=" + userId + " ";
        // ************************************************************************
        // 注意：不能用以下方式创建UserConfigInfo对象。这样只能创建出基类对象，无法创建派生类对象
        // UserConfigInfo userConfigInfo = mFactory.createFromProtoBuf(data);
        // UserConfigInfo userConfigInfo = new UserConfigInfo();
        // ************************************************************************

        // 此处使用传入的UserConfigFactory对象创建对应的UserConfigInfo对象。创建的可能是派生类对象
        UserConfigInfo userConfigInfo = mFactory.create();

        // 对于无效ID，直接返回
        if (userId <= 0) {
            Oce.getLogger().error(callInfo + "User id is invalid");
            return userConfigInfo;
        }

        // 1. 从TripodCache获取数据
        byte[] data = null;
        try {
            data = mTripodClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(callInfo + "Ice.TimeoutException");
            throw e;
        } catch (Exception e) {
            Oce.getLogger().error(callInfo + "Exception\n" + e.getLocalizedMessage());
            return userConfigInfo;
        }

        // 2. 判断取回的数据是否有异常，若有异常直接返回
        if (data == null) {
            Oce.getLogger().error(callInfo + "Protocol buffer is null!");
            return userConfigInfo;
        } else if (data.length == 0) {
            Oce.getLogger().error(callInfo + "Protocol Buffer String length is zero!");
            return userConfigInfo;
        }

        // 3. 尝试反解取回的数据
        UserConfigData userConfigData = null;
        try {
            userConfigData = UserConfigData.parseFrom(data);
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(callInfo + "Protocol Buffer String is invalid!");
            return userConfigInfo;
        } catch (Exception e) {
            Oce.getLogger().error(callInfo + "Exception\n" + e.getLocalizedMessage());
            return userConfigInfo;
        }

        // 4. 判断反解出的数据是否有异常，若有异常则直接返回或抛出异常
        if (userConfigData == null) {
            Oce.getLogger().error(callInfo + "UserConfigData is null!");
            return userConfigInfo;
        } else if (userConfigData.getId() == 0) {
            // 异常，DB中没有该用户数据
            Oce.getLogger().error(callInfo + " No such user!");
            throw new Ice.UnknownUserException();
        }

        // 5. 调用基类方法，从ProtoBuf对象获取相关字段的值
        userConfigInfo.parseFromProtoObj(userConfigData);
        return userConfigInfo;
    }

    // 抛出 Ice.TimeoutException
    public boolean reloadUserConfig(int userId) {
        String callInfo = this.getClass().getName() + ".reloadUserConfig  id=" + userId + " ";

        if (userId <= 0) return false;

        try {
            return mTripodClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(callInfo + "Ice.TimeoutException");
            throw e;
        } catch (Exception e) {
            Oce.getLogger().error(callInfo + "Exception\n" + e.getLocalizedMessage());
            return false;
        }
    }
}

package com.xiaonei.xce.userbaseview;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import xce.tripod.MultiBizTripodClient;
import xce.tripod.TripodException;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.tripod.proto.UserBaseProto.PbUserBorn;
import com.renren.tripod.proto.UserBaseProto.PbUserName;
import com.renren.tripod.proto.UserBaseProto.PbUserPassport;
import com.renren.tripod.proto.UserBaseProto.PbUserRight;
import com.renren.tripod.proto.UserBaseProto.PbUserStage;
import com.renren.tripod.proto.UserBaseProto.PbUserState;
import com.renren.tripod.proto.UserBaseProto.PbUserUrl;
import com.xiaonei.xce.idcache.IdInfoAdapter;
import com.xiaonei.xce.log.Oce;
import com.xiaonei.xce.log.TimeCost;
import com.xiaonei.xce.userdanger.UserDangerInfo;
import com.xiaonei.xce.username.UserNameFactory;
import com.xiaonei.xce.username.UserNameInfo;
import com.xiaonei.xce.userpassport.UserPassportFactory;
import com.xiaonei.xce.userpassport.UserPassportInfo;
import com.xiaonei.xce.userright.UserRightFactory;
import com.xiaonei.xce.userright.UserRightInfo;
import com.xiaonei.xce.userstate.UserStateFactory;
import com.xiaonei.xce.userstate.UserStateInfo;
import com.xiaonei.xce.userurl.UserUrlFactory;
import com.xiaonei.xce.userurl.UserUrlInfo;

public class UserBaseCommonViewTripodAdapter<T extends UserBaseCommonView<P, R, N, S, U>, //NL
P extends UserPassportInfo, R extends UserRightInfo, N extends UserNameInfo, //NL
S extends UserStateInfo, U extends UserUrlInfo> {

    protected final UserBaseCommonViewFactory<T> _viewFactory;

    protected final UserPassportFactory<P> _passportFactory;

    protected final UserRightFactory<R> _rightFactory;

    protected final UserNameFactory<N> _nameFactory;

    protected final UserStateFactory<S> _stateFactory;

    protected final UserUrlFactory<U> _urlFactory;

    private MultiBizTripodClient _client;

    private final static String NAME_SPACE = "xce_user";

    private final static String USER_BORN = "user_born";

    private final static String USER_STAGE = "user_stage";

    private final static String USER_NAME = "user_name";

    private final static String USER_URL = "user_url";

    private final static String USER_PASSPORT = "user_passport";

    private final static String USER_STATE = "user_state";

    private final static String USER_RIGHT = "user_right";

    private final static String USER_LOGINCOUNT = "user_logincount";

    private final static int EXPIRE_TIME = 3600 * 24 * 30; //一个月

    private final static List<String> BIZS;

    static {
        BIZS = new ArrayList<String>();
        BIZS.add(USER_BORN);
        BIZS.add(USER_STAGE);
        BIZS.add(USER_NAME);
        BIZS.add(USER_URL);
        BIZS.add(USER_PASSPORT);
        BIZS.add(USER_STATE);
        BIZS.add(USER_RIGHT);
        BIZS.add(USER_LOGINCOUNT);
    }

    private IdInfoAdapter _idInfoAdapter = null;

    public UserBaseCommonViewTripodAdapter(UserBaseCommonViewFactory<T> viewFactory,
            UserPassportFactory<P> passportFactory, UserRightFactory<R> rightFactory,
            UserNameFactory<N> nameFactory, UserStateFactory<S> stateFactory,
            UserUrlFactory<U> urlFactory) {
        _viewFactory = viewFactory;
        _passportFactory = passportFactory;
        _rightFactory = rightFactory;
        _nameFactory = nameFactory;
        _stateFactory = stateFactory;
        _urlFactory = urlFactory;
        _idInfoAdapter = IdInfoAdapter.getInstance();
        _client = new MultiBizTripodClient(NAME_SPACE);
    }

    public T getUserBaseView(int userId) {
        TimeCost cost = TimeCost
                .begin("UserBaseCommonViewTripodAdapter.getUserBaseViewTripod userId=" + userId);
        T result = _viewFactory.create();
        if (userId <= 0) {
            return result;
        }
        int realUserId = userId;

        try {
            if (_idInfoAdapter.isVirtualId(userId)) {
                int relatedTransId = _idInfoAdapter.getRelatedTransId(userId);
                if (relatedTransId > 0) {
                    int relatedUserId = _idInfoAdapter.getRelatedUserId(userId);
                    if (relatedUserId > 0) {
                        realUserId = relatedUserId;
                    }
                    userId = relatedTransId;
                }
            }
            result.setRealUserId(realUserId);

            String key = String.valueOf(userId);
            byte[] dataRaw = null;
            Map<String, byte[]> dataRaws = null;
            try {
                dataRaws = _client.multiBizGet(key, BIZS, EXPIRE_TIME);
            } catch (TripodException e) {
                Oce.getLogger().error("UserBaseCommonViewTripodAdapter.getUserBaseView userId=" //NL
                        + userId + " TripodException msg:" + e.getMessage());
            }

            dataRaw = dataRaws.get(USER_PASSPORT);
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserPassport passportDataPb = PbUserPassport.parseFrom(dataRaw);
                //只对passport检查抛异常，如果passport没问题认为是有这个id的
                if (passportDataPb.getId() == 0) {
                    Oce.getLogger().error("UserBaseCommonViewTripodAdapter.getUserBaseView userId=" //NL
                            + userId + " No such user");
                    throw new Ice.UnknownUserException();
                }
                P passportInfo = _passportFactory.create();
                passportInfo.parse(passportDataPb);
                result.setUserPassportInfo(passportInfo);
            }
            dataRaw = dataRaws.get(USER_BORN);
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserBorn bornDataPb = PbUserBorn.parseFrom(dataRaw);
                result.setGender(bornDataPb.getGender());
            }
            dataRaw = dataRaws.get(USER_STAGE);
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserStage stageDataPb = PbUserStage.parseFrom(dataRaw);
                result.setStage(stageDataPb.getStage());
                result.setUniv(stageDataPb.getUniv());
            }
            dataRaw = dataRaws.get(USER_NAME);
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserName nameDataPb = PbUserName.parseFrom(dataRaw);
                N nameInfo = _nameFactory.create();
                nameInfo.parse(nameDataPb);
                result.setUserNameInfo(nameInfo);
            }
            dataRaw = dataRaws.get(USER_URL);
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserUrl urlDataPb = PbUserUrl.parseFrom(dataRaw);
                U urlInfo = _urlFactory.create();
                urlInfo.parse(urlDataPb);
                result.setUserUrl(urlInfo);
            }
            dataRaw = dataRaws.get(USER_STATE);
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserState stateDataPb = PbUserState.parseFrom(dataRaw);
                S stateInfo = _stateFactory.create();
                stateInfo.parse(stateDataPb);
                result.setUserStateInfo(stateInfo);
            }
            dataRaw = dataRaws.get(USER_RIGHT);
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserRight rightDataPb = PbUserRight.parseFrom(dataRaw);
                R rightInfo = _rightFactory.create();
                rightInfo.parse(rightDataPb);
                result.setUserRightInfo(rightInfo);
            }
            dataRaw = dataRaws.get(USER_LOGINCOUNT);
            if (dataRaw != null && dataRaw.length > 0) {
                try {
                    result.setUserLoginCountInfo((int) Long.parseLong(new String(dataRaw)));
                } catch (NumberFormatException e) {
                    result.setUserLoginCountInfo(0);
                    Oce.getLogger().error("UserBaseCommonViewTripodAdapter.getUserBaseView userId=" //NL
                            + userId + " NumberFormatException");
                }
            }

            result.setUserDangerInfo(new UserDangerInfo(userId));

        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error("UserBaseCommonViewTripodAdapter.getUserBaseView userId=" //NL
                    + userId + " InvalidProtocolBuf");
        } finally {
            cost.endFinally();
        }
        return result;
    }

}

package com.xiaonei.xce.userbaseview;

import xce.userbase.UserBaseViewManagerPrx;
import xce.userbase.UserBaseViewManagerPrxHelper;
import xce.userbase.UserCommonViewDataN;
import Ice.ConnectTimeoutException;
import Ice.TimeoutException;

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

//import com.xiaonei.xce.username.DefaultUserNameFactory;
//import com.xiaonei.xce.username.DefaultUserNameInfo;
//import com.xiaonei.xce.userpassport.DefaultUserPassportFactory;
//import com.xiaonei.xce.userpassport.DefaultUserPassportInfo;
//import com.xiaonei.xce.userright.DefaultUserRightFactory;
//import com.xiaonei.xce.userright.DefaultUserRightInfo;
//import com.xiaonei.xce.userstate.DefaultUserStateFactory;
//import com.xiaonei.xce.userstate.DefaultUserStateInfo;
//import com.xiaonei.xce.userurl.DefaultUserUrlFactory;
//import com.xiaonei.xce.userurl.DefaultUserUrlInfo;

import xce.clusterstate.ReplicatedClusterAdapter;

public class UserBaseCommonViewAdapter<T extends UserBaseCommonView<P, R, N, S, U>, P extends UserPassportInfo, R extends UserRightInfo, N extends UserNameInfo, S extends UserStateInfo, U extends UserUrlInfo>
        extends ReplicatedClusterAdapter {

    private static final String ctr_endpoints = "ControllerUserBase";

    private static final String service_name = "UBVM";

    private static int _interval = 120;

    protected final UserBaseCommonViewFactory<T> _viewFactory;

    protected final UserPassportFactory<P> _passportFactory;

    protected final UserRightFactory<R> _rightFactory;

    protected final UserNameFactory<N> _nameFactory;

    protected final UserStateFactory<S> _stateFactory;

    protected final UserUrlFactory<U> _urlFactory;

    private IdInfoAdapter _idInfoAdapter = null;

    private UserBaseCommonViewTripodAdapter<T, P, R, N, S, U> _tripodAdapter;

    public UserBaseCommonViewAdapter(UserBaseCommonViewFactory<T> viewFactory,
            UserPassportFactory<P> passportFactory, UserRightFactory<R> rightFactory,
            UserNameFactory<N> nameFactory, UserStateFactory<S> stateFactory,
            UserUrlFactory<U> urlFactory) {
        super(ctr_endpoints, _interval);
        _viewFactory = viewFactory;
        _passportFactory = passportFactory;
        _rightFactory = rightFactory;
        _nameFactory = nameFactory;
        _stateFactory = stateFactory;
        _urlFactory = urlFactory;
        _idInfoAdapter = IdInfoAdapter.getInstance();
        _tripodAdapter = new UserBaseCommonViewTripodAdapter<T, P, R, N, S, U>(viewFactory,
                passportFactory, rightFactory, nameFactory, stateFactory, urlFactory);
    }

    private T getUserBaseViewIce(int userId) {
        TimeCost cost = TimeCost.begin("UserBaseCommonViewAdapter.getUserBaseView id: " + userId);

        T res = _viewFactory.create();
        UserBaseViewManagerPrx prx = null;
        int realUserId = userId;
        try {
            if (userId <= 0) {
                return res;
            }

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

            prx = getUserBaseViewManager(userId);
            UserCommonViewDataN ubvd = prx.getUserCommonViewN(userId);
            res.setRealUserId(realUserId);
            P passport = _passportFactory.create();
            passport.parse(ubvd.userpassport);
            res.setUserPassportInfo(passport);

            R right = _rightFactory.create();
            right.parse(ubvd.userright);
            res.setUserRightInfo(right);

            N name = _nameFactory.create();
            name.parse(ubvd.username);
            res.setUserNameInfo(name);

            S state = _stateFactory.create();
            state.parse(ubvd.userstate);
            res.setUserStateInfo(state);

            res.setUserLoginCountInfo(ubvd.userlogincount.logincount);

            U url = _urlFactory.create();
            url.parse(ubvd.userurl);
            res.setUserUrl(url);

            res.setStage(ubvd.stage);
            res.setUniv(ubvd.univ);
            res.setGender(ubvd.gender);
            res.setUserDangerInfo(new UserDangerInfo(userId));
            return res;
        } catch (ConnectTimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUserBaseView [" + prx + "] userId=" + userId
                            + " Ice.ConnectTimeoutException");
            throw e;
        } catch (TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUserBaseView [" + prx + "] userId=" + userId
                            + " Ice.TimeoutException");
            throw e;
        } finally {
            cost.endFinally();
        }
    }

    private T getUserBaseViewTripod(int userId) {
        return _tripodAdapter.getUserBaseView(userId);
    }

    private boolean userTripod(int userId) {
        if (userId % 10 == 7) {
            return true;
        } else {
            return false;
        }
    }

    public T getUserBaseView(int userId) {
        if (userTripod(userId)) {
            return getUserBaseViewTripod(userId);
        } else {
            return getUserBaseViewIce(userId);
        }
    }

    protected UserBaseViewManagerPrx getUserBaseViewManager(int userId) {
        Ice.ObjectPrx prx0 = getValidProxy(userId, service_name, 300);
        if (prx0 != null) {
            UserBaseViewManagerPrx prx = UserBaseViewManagerPrxHelper.uncheckedCast(prx0
                    .ice_timeout(300).ice_twoway());
            return prx;
        } else {
            return null;
        }
    }
    
//    public static void main(String[] args) {
//        class MUserBaseCommonView extends UserBaseCommonView<DefaultUserPassportInfo, //NL
//                DefaultUserRightInfo, DefaultUserNameInfo, DefaultUserStateInfo, DefaultUserUrlInfo> {
//        };
//        class MUserBaseCommonViewFactory implements UserBaseCommonViewFactory<MUserBaseCommonView> {
//
//            public MUserBaseCommonView create() {
//                return new MUserBaseCommonView();
//            }
//        };
//        MUserBaseCommonViewFactory viewFactory = new MUserBaseCommonViewFactory();
//        DefaultUserPassportFactory passportFactory = new DefaultUserPassportFactory();
//        DefaultUserRightFactory rightFactory = new DefaultUserRightFactory();
//        DefaultUserNameFactory nameFactory = new DefaultUserNameFactory();
//        DefaultUserStateFactory stateFactory = new DefaultUserStateFactory();
//        DefaultUserUrlFactory urlFactory = new DefaultUserUrlFactory();
//
//        UserBaseCommonViewAdapter<MUserBaseCommonView, DefaultUserPassportInfo, //NL
//        DefaultUserRightInfo, DefaultUserNameInfo, DefaultUserStateInfo, DefaultUserUrlInfo> adapter //NL
//        = new UserBaseCommonViewAdapter<MUserBaseCommonView, DefaultUserPassportInfo, //NL
//        DefaultUserRightInfo, DefaultUserNameInfo, DefaultUserStateInfo, DefaultUserUrlInfo>(
//                viewFactory, passportFactory, rightFactory, nameFactory, stateFactory, urlFactory);
//
//        int userId = 35178;
//
//        MUserBaseCommonView info = viewFactory.create();
//        try {
//            info = adapter.getUserBaseView(userId);
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//
//        System.out.println(info.toString());
//
//        System.exit(0);
//    }

}

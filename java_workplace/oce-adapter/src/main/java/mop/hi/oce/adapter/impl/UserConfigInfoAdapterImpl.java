package mop.hi.oce.adapter.impl;

import mop.hi.oce.domain.user.UserConfigInfo;
import mop.hi.oce.domain.user.UserConfigInfoFactory;
import mop.hi.svc.model.UserConfigManagerPrx;
import mop.hi.svc.model.UserException;

/**
 * <p>
 * Title: UserConfigInfoAdapterImpl.java
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author xin.huang@renren-inc.com
 * @date 2012-7-11
 */
public class UserConfigInfoAdapterImpl {

    private UserConfigInfoReaderAdapterImpl _readerAdapter = null;

    private UserConfigInfoWriterAdapterImpl _writerAdapter = null;

    // 默认构造函数，使用默认factory产生数据对象
    public UserConfigInfoAdapterImpl() {
        this(new UserConfigInfoFactory());
    }

    // 使用传入的factory产生数据对象
    public UserConfigInfoAdapterImpl(UserConfigInfoFactory factory) {
        _readerAdapter = new UserConfigInfoReaderAdapterImpl(factory);
        _writerAdapter = new UserConfigInfoWriterAdapterImpl();
    }

    /**
     * 创建用户的设置数据
     * 
     * @param userId 用户ID
     * @param uc 用户设置数据
     */
    public void createUserConfig(int userId, UserConfigInfo uc) {
        _writerAdapter.createUserConfig(userId, uc);
    }

    /**
     * 获取用户的设置数据
     * 
     * @param userId 用户ID
     * @return UserConfigInfo 用户设置数据
     * @throws UserException DB中没有该用户时抛出
     * @throws Exception
     */
    public UserConfigInfo getUserConfig(int userId) throws UserException, Exception {
        return _readerAdapter.getUserConfig(userId);
    }

    /**
     * 保存用户的设置数据
     * 
     * @param userId 用户ID
     * @param uc 用户设置数据
     */
    public void setUserConfig(int userId, UserConfigInfo uc) {
        _writerAdapter.setUserConfig(userId, uc);
    }

    /**
     * 重新加载缓存中的用户设置数据
     * 
     * @param userId 用户ID
     */
    public void reloadUserConfig(int userId) {
        _writerAdapter.reloadUserConfig(userId);
    }

    public static boolean useTripod(int userId) {
        int mod = userId % 10;
        switch(mod){
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
            return true;
          default:
            return false;
        }
    }

    /**
     * 安全起见，本接口不再对外开放，如有需要用到，请联系中间层： renren.xce@renren-inc.com
     * 
     * @param userId
     * @return null
     */
    @Deprecated
    public UserConfigManagerPrx getUserConfigManager(int userId) {
        return null;
    }

    /**
     * 安全起见，本接口不再对外开放，如有需要用到，请联系中间层： renren.xce@renren-inc.com
     * 
     * @param userId
     * @return null
     */
    @Deprecated
    public UserConfigManagerPrx getUserConfig10sManager(int userId) {
        return null;
    }

    public static void main(String[] args) {
        System.out.println("args:userId");
        int userId = 349440824;
        if (args.length == 1) {
            userId = Integer.parseInt(args[0]);
        }
        UserConfigInfoAdapterImpl userConfigInfo = new UserConfigInfoAdapterImpl();
        System.out.println("networkAdapter : " + userConfigInfo);
        try {
            userConfigInfo.reloadUserConfig(userId);
            Thread.sleep(1000);

            UserConfigInfo uci = userConfigInfo.getUserConfig(userId);

            System.out.println(uci);

            //            com.xiaonei.platform.core.model.UserConfigInfo info = WUserConfigInfoAdapter
            //                    .getInstance().getUserConfig(userId);
            //
            //            System.out.println(info);

            // uci.setWantSeeCss(1);
            // userConfigInfo.setUserConfig(userId, uci);
            // uci = userConfigInfo.getUserConfig(userId);
            // System.out.println(uci);
        } catch (Ice.UnknownUserException e) {
            System.out.println("[WARN]No such user, userId=" + userId);
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.exit(0);
    }

}

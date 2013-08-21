/**
 *
 */
package com.xiaonei.reg.register.logic;

import com.xiaonei.reg.register.pojo.RegisterPojo;

/**
 * 大家好，我是不用审核注册后台的接口
 * <p/>
 * RegisterLogic.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-25 下午12:06:46
 */
public interface IRegisterLogic {
    /**
     * 错误代码——无错
     */
    public static final int err_noerr = 0;
    /**
     * 错误代码——未知错误
     */
    public static final int err_unknown = 1;
    /**
     * 错误代码——user config 错误
     */
    public static final int err_user_config = 2;
    /**
     * 错误代码——加入网络错误
     */
    public static final int err_join_network = 4;
    /**
     * 错误代码——初始化信息错误
     */
    public static final int err_initialize_info = 8;
    /**
     * 错误代码——发送激活新
     */
    public static final int err_send_activate_email = 16;
    /**
     * 错误代码——记录高中学校信息
     */
    public static final int err_hs_log = 32;
    /**
     * 错误代码——非法邀请链接 sql exception引起
     */
    public static final int err_invalidate_invite_link_sql = 64;
    /**
     * 错误代码——非法邀请链接
     */
    public static final int err_invalidate_invite_link = 128;
    /**
     * 错误代码——把邀请人与被邀请人保存入数据库失败
     */
    public static final int err_init_inviter_invitee_failure = 256;
    /**
     * 错误代码——新鲜事
     */
    public static final int err_news_feed = 512;
    /**
     * 错误代码——激活
     */
    public static final int err_activate = 1024;
    /**
     * 错误代码——激活信
     */
    public static final int err_send_activate_mail = 2048;
    /**
     * 错误代码——无错误-自动激活
     */
    public static final int err_noerr_auto_activate = 4096;

    /**
     * 注册的关键部分<br>
     * 实例化子类的控制器只需调用此方法
     *
     * @param reg 注册传
     * @return 用户id(负)
     * @throws Exception
     * @author (wang-tai)tai.wang@opi-corp.com 2008-9-25 下午12:19:42
     */
    public int register(RegisterPojo reg) throws Exception;
}

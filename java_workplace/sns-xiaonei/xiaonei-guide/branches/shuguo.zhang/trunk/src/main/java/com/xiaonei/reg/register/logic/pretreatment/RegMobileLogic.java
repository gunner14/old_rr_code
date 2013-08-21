/**
 *
 */
package com.xiaonei.reg.register.logic.pretreatment;

import java.sql.SQLException;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.component.mobile.MobileBindConstants;
import com.xiaonei.platform.component.mobile.model.SetMobileUser;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.VerifyCode;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.common.users.UserInfo;
import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.dao.RegMobileWebInfoMapDAO;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckRegEmail;
import com.xiaonei.reg.register.model.RegMobileWebInfoMapHS;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.smsurl.logic.FlowMtLogic;
import com.xiaonei.smsurl.system.Constants;

/**
 * RegMobileLogic.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-11 下午06:31:39
 */
public class RegMobileLogic {

    private final static String MEM_CACHE_REG_MOBILE_WEB = "mem_cache_reg_mobile_web_";

    private final RegLogger rlog = RegLogger.getLoggerWithType(this.getClass());
    
    private static RegMobileLogic _instance = new RegMobileLogic();

    public static RegMobileLogic getInstance() {
        return _instance;
    }

    private RegMobileLogic() {
        super();
    }

    /**
     * 下行短信
     */
    private static final String DOWN_SMS_1 = "您的人人网注册验证码为：";

    private static final String DOWN_SMS_2 = "，请在网页输入此验证码完成注册。人人网关怀提示：不要将验证码透露给其他人。";

    /**
     * 1.发送下行短信<br>
     * 您的临时验证码：xxxx,请按页面提示填写验证码开通账户<br>
     * 2.保存用户信息<br>
     * 
     * @param RegisterPojo
     * @return downCode
     * @throws Exception
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 上午11:13:47
     */
    public String sendDownCodeAndSaveInfo(RegisterPojo rp) throws Exception {
        String mobile = rp.getRegEmail();

        // 获取验证码
        // 发送短信
        String downCode = sendSms(mobile);
        // look look
        if (null != downCode) {
            // 　save
            this.addRegMobileInfo_hs(new RegMobileWebInfoMapHS(rp, downCode));
        } else {
            return null;
        }
        return downCode;
    }

    public String sendSms(String mobile) throws Exception {
        if (!canSend(mobile)) {
            return null;
        }
        String downCode = this.getNewDownCode(mobile);
        if (null != downCode) {
            FlowMtLogic.sendSms((long) 0, "000020", "10", mobile, DOWN_SMS_1 + downCode
                    + DOWN_SMS_2, "", Constants.FLOW_WEB_REG1);
            rlog.debug("mobile register code::: mo=" + mobile + ",code=" + downCode);
            return downCode;
        } else {
            throw new Exception("make verify code fail");
        }
    }

    /**
     * 发送短信限制
     * 
     * @param mobile
     * @return times
     * @author wang-tai(tai.wang@opi-corp.com) 2009-3-10 - 下午09:29:15
     */
    private boolean canSend(String mobile) {
        if (!CheckRegEmail.OK.equals(new CheckRegEmail().check(mobile, IAccountType.MOBILE))) {
            return false;
        }

        MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(
                MemCacheKeys.pool_user_space);
        String key = MEM_CACHE_REG_MOBILE_WEB + mobile;
        Object obj = mem.get(key);
        int times;
        try {
            times = ((Integer) obj).intValue();
        } catch (NullPointerException e) {
            times = 0;
        }
        if (times == 0) {
            mem.set(key, ++times, (int) (MemCacheKeys.day));
        } else {
            mem.replace(key, ++times);
        }
        rlog.debug("mobile register code::: mo=" + mobile + ", send times="+ times);
        return (times <= 3);
    }

    /**
     * 产生或获得下发短信
     * 
     * @param mobile
     * @return 成功返回验证码，出错 null;
     * @author wang-tai(tai.wang@opi-corp.com) 2008-11-28 - 上午10:30:14
     */
    private String getNewDownCode(String mobile) {
        String mo = null;
        try {
            mo = RegMobileWebInfoMapDAO.getInstance().getDownCode(mobile);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        if (null != mo) {
            return StringUtils.lowerCase(mo);
        } else {
            return StringUtils.lowerCase(VerifyCode.getVerifyCode(5));
        }
    }

    public boolean checkDownCode(String mobile, String downCode) {
        String mo = null;
        try {
            mo = RegMobileWebInfoMapDAO.getInstance().getDownCode(mobile);
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
        if (StringUtils.equals(mo, downCode)) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * 获得mobile注册临时数据
     * 
     * @param mobile
     * @param downCode
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-12 下午12:09:18
     */
    public RegMobileWebInfoMapHS getRegMobileInfoByCode_hs(final String mobile, String downCode)
            throws SQLException {
        downCode = StringUtils.lowerCase(downCode);
        RegMobileWebInfoMapHS rm = RegMobileWebInfoMapDAO.getInstance()
                .getRegMobileInfoByMobileAndCode_hs(mobile, downCode);
        return rm;
    }

    /**
     * 保存mobile注册临时表
     * 
     * @param rm
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-12 上午11:56:26
     */
    private int addRegMobileInfo_hs(final RegMobileWebInfoMapHS rm) throws SQLException {
        RegMobileWebInfoMapHS rmwi = this.getRegMobileInfoByCode_hs(rm.getMobile(), rm
                .getDown_code());
        if (null != rmwi) {
            RegMobileWebInfoMapDAO.getInstance().delRegMobileInfo(rmwi.getId());
        }
        return RegMobileWebInfoMapDAO.getInstance().AddRegMobileInfo_hs(rm);
    }

    /**
     * 保存验证邮件
     * 
     * @param host
     * @param email
     * @return 成功 1
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-8 - 下午03:42:25
     */
    public String saveEmail(User host, String email) {
        email = email.trim().toLowerCase();

        UserInfo u = UserAdapter.get(host.getId());
        if (!email.equalsIgnoreCase(u.getNewEmail())) {
            String emailVc = VerifyCode.getVerifyCode(10);
            UserAdapter.updateVerifyNewEmail(host.getId(), email, emailVc);
            return emailVc;
        } else {
            return "";
        }
    }

    /**
     * 手机绑定 和 创建彩信相册
     * 
     * @param user
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-22 - 下午05:37:40
     */
    public void bindMobileAndCreateMMSAlbum(User user) {
        SetMobileUser mu = new SetMobileUser();
        mu.setUesrid(user.getId());
        mu.setMobile(user.getAccount());
        mu.setVerifyCode("web_reg");
        mu.setStatus(1);
        mu.setSource(MobileBindConstants.WEB_REGISTER);
        try {
            com.xiaonei.platform.component.mobile.logic.MobileLogic.getInstance().bind(mu);
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            //com.xiaonei.album.logic.AlbumLogic.getInstance().getMMS(user);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 判断手机号是否已经绑定
     * 
     * @param mobile
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-3-16 - 下午04:14:41
     */
    public boolean isBinded(String mobile) {
        SetMobileUser muser = com.xiaonei.platform.component.mobile.logic.MobileLogic.getInstance()
                .getMobileUser(mobile);
        if (muser == null) return false;
        return (MobileBindConstants.WAP_BIND == muser.getStatus() || MobileBindConstants.WEB_BIND == muser
                .getStatus());
    }

}

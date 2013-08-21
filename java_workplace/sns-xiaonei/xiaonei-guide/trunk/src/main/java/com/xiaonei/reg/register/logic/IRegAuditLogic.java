/**
 *
 */
package com.xiaonei.reg.register.logic;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.pojo.AuditBasePojo;

/**
 * 注册审核
 * <p/>
 * RegAuditLogic.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-9 下午04:09:15
 */
public interface IRegAuditLogic {

    /**
     * 通知审核有新用户需要审核
     *
     * @param reg
     * @return isWait 等待页面，err_ 出错
     * @author (wang-tai)tai.wang@opi-corp.com 2008-10-10 上午10:43:32
     */
    public int audit(AuditBasePojo audit);

    /**
     * 判断一个用户是否应该被审核
     *
     * @param regForm
     * @param ip
     * @return isReg 不用审核，isAudit用审核，其他 判断过程出错
     * @author (wang-tai)tai.wang@opi-corp.com 2008-10-10 上午10:47:26
     */
    public int shouldBeAudit(BaseRegForm regForm, String ip);

    public static final int isReg = 0;
    public static final int isAudit = 1;
    public static final int isWait = -1;//负数

    public static final int exception_sql = 50;

    public static final int err_system = 100;
    public static final int err_no_choice_univ = 101;
    public static final int err_no_such_univ = 102;
    public static final int err_no_department = 103;
    public static final int err_no_dormitory = 104;
    public static final int err_no_portrait = 105;
    public static final int err_no_enrollyear = 106;
    public static final int err_no_class = 107;

    public static final int err_invalid_portrait_format = 120;
    public static final int err_invalid_email = 121;


    /**
     * 头像存储要保留在天津,没有办法写北京的数据库
     */
    public final static boolean UPLOAD_FUN_TEMP = true;
}

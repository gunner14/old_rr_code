/**
 *
 */
package com.xiaonei.reg.register.logic;


/**
 * hi,小明，我是mobile 注册的接口
 * <p/>
 * IRegMobileLogic.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-10 下午06:50:53
 */
public interface IRegMobileLogic extends IRegisterLogic {

    public static final int check_down_code_false = -1;
    public static final int sys_err = -2;
    public static final int no_user = -3;
    public static final int user_exist = -4;

    /**
     * 1.检查验证码<br>
     * 2.生成用户
     *
     * @param mobileNum
     * @param downCode
     * @return 用户id,-1 请重新填写验证码, -2 系统错误,请稍后重试
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 上午11:13:47
     */
    public int checkDownCodeAndReg(String mobileNum, String downCode);

    /**
     * 检查上行短信后生成用户,远程调用
     *
     * @param mobileNum
     * @return 用户id 出错 负数
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-11 下午08:07:54
     */
    public int regAfterCheckUpCode(String mobileNum);
	
}

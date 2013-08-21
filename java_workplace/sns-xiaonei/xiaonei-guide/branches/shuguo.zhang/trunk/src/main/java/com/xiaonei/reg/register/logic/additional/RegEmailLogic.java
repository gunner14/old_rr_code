package com.xiaonei.reg.register.logic.additional;

import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.email.service.control.ProfileEmail;
import com.xiaonei.email.service.model.EmailConstants;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.dao.VerifyCodeDAO;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.dao.EmailDAO;
import com.xiaonei.reg.register.model.Email;
import com.xiaonei.reg.register.pojo.RegisterPojo;

/**
 * 负责发送激活信等等
 * 
 * @author wang-tai
 */
public class RegEmailLogic {

    private static RegEmailLogic _instance = new RegEmailLogic();

    public static RegEmailLogic getInstance() {
        return _instance;
    }

    private RegEmailLogic() {
        super();
    }

    public void sendActivateEmail(int userId) {
        this.sendActivateEmail(UserAdapter.getUser(userId));
    }

    public void sendActivateEmail(User user, RegisterPojo regPojo) {
        if(regPojo == null){
            this.sendActivateEmail(user);
        }
        if (user == null) {
            return;
        }
        try {
            if("205226".equals(regPojo.getActionId())){ //游戏注册用户
                this.sendGameRegActivateEmail(user);
            }else if("206224".equals(regPojo.getActionId())
                    || "206225".equals(regPojo.getActionId())){ //page注册用户
                this.sendPageActivateEmail(user, regPojo.getPageId());
            }            
            else{
                this.sendNoneActivateEmail(user);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public void sendActivateEmail(User user) {
        if (user == null) {
            return;
        }
        try {
            this.sendNoneActivateEmail(user);
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 发送普通邀请信，none
     * 
     * @param user
     * @throws SQLException
     */
    private void sendNoneActivateEmail(User user) throws SQLException {
        String link = getActivateLink(user);
        sendNoneActivateEmail(user, link);
    }

    /**
     * page 注册需要pageid参数
     * @param user
     * @param pageId
     * @throws SQLException
     * @author wei.cheng@opi-corp.com
     */
    public void sendPageActivateEmail(User user, int pageId) throws SQLException{
        String link = getActivateLinkPage(user, pageId);
        sendNoneActivateEmail(user, link);
    }    
    
    private void sendNoneActivateEmail(User user, String link) throws SQLException {
        if(StringUtils.isEmpty(link)){
            link = getActivateLink(user);
        }
        StringBuffer buf = new StringBuffer();
        buf.append(user.getName());
        buf.append("，你好：<br/><br/>");
        buf
                .append("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;感谢你注册人人网(<a href=\"http://www."+Globals.domain+"\">www."+Globals.domain+"</a>)" +
                        "，点击下面的链接即可完成注册：<br/><br/>" +
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='"
                        + link + "'>" + link);
        buf.append("</a>");
        buf.append("<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(如果链接无法点击，请将它拷贝到浏览器的地址栏中。)<br/>");
        buf.append("<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;你的人人网帐号是：" + user.getAccount() + "<br/>");
        buf.append("<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;人人网" +
                    "<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
        Calendar c = Calendar.getInstance();
        buf.append(c.get(Calendar.YEAR) + "年" + (c.get(Calendar.MONTH) + 1) + "月"
                + c.get(Calendar.DAY_OF_MONTH) + "日");

        Email em = new Email(user.getAccount(), Globals.systemEmail, "人人网(www."+Globals.domain+")注册确认", buf.toString());
        em.setDisc(10000001);
        EmailDAO.getInstance().save(em);
    }
    
    /**
     * 游戏注册用户
     * @param user
     * @throws SQLException
     * @author weige
     */
    private void sendGameRegActivateEmail(User user) throws SQLException {
        StringBuffer buf = new StringBuffer();
        buf.append(user.getName());
        buf.append("，你好：<br/><br/>");
        buf
                .append("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;感谢你注册武侠风云" +
                        "，点击下面的链接可快速进入：<br/><br/>" +
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='"
                        + getActivateLink(user) + "'>" + getActivateLink(user));
        buf.append("</a>");
        buf.append("<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(如果链接无法点击，请将它拷贝到浏览器的地址栏中。)<br/>");
        buf.append("<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;你的人人网游戏通行证是：" + user.getAccount() + "<br/>");
        buf.append("<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;人人网" +
                    "<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
        Calendar c = Calendar.getInstance();
        buf.append(c.get(Calendar.YEAR) + "年" + (c.get(Calendar.MONTH) + 1) + "月"
                + c.get(Calendar.DAY_OF_MONTH) + "日");

        Email em = new Email(user.getAccount(), Globals.systemEmail, "人人网(www."+Globals.domain+")注册确认", buf.toString());
        em.setDisc(10000001);
        EmailDAO.getInstance().save(em);
    }
    
    public String getLinkforPub(String email) {
        User user = UserAdapter.getUser(email, Globals.domain.toString());
        return getActivateLink(user);
    }

    public String getLinkforPub(int userId) {
        User user = UserAdapter.getUser(userId);
        return getActivateLink(user);
    }

    public String getLinkforPage(String email, String pageId) {
        int pid = 0;
        if(StringUtils.isNotEmpty(pageId) && StringUtils.isNumeric(pageId)){
            pid = Integer.parseInt(pageId);
        }
        User user = UserAdapter.getUser(email, Globals.domain.toString());
        return getActivateLinkPage(user, pid);
    }
    /**
     * page注册使用激活链接
     * @param userId
     * @param pageId
     * @return
     * @author 程伟 Email:wei.cheng@opi-corp.com
     * @version 创建时间：Mar 8, 2010 2:29:36 PM
     */
    public String getLinkforPage(int userId, int pageId) {
        User user = UserAdapter.getUser(userId);
        return getActivateLinkPage(user, pageId);
    }
    
    private String getActivateLinkPage(User user, int pageId) {
        return Globals.urlReg.toString() + "/activate.do?id=" + user.getId() + "&vc="
        + UserAdapter.get(user).getVerifyCode() + "&pageid=" + pageId;
    }

    private String getActivateLink(User user) {
        return Globals.urlReg.toString() + "/activate.do?id=" + user.getId() + "&vc="
                + UserAdapter.get(user).getVerifyCode();
    }

    /**
     * 手机注册用户的邮箱验证信<br>
     * xxx，你好：<br>
     * 点击下面的链接即可完成邮箱验证.<br>
     * <br>
     * 验证成功后，你的人人网帐号将更改为：xxxxxx@xxx.com ，手机号码仍可登录人人网。<br>
     * <br>
     * /activate.do?id=261407210<br>
     * /activate.do?id=261407210&vc=lUUm4vlRfa><br>
     * &vc=lUUm4vlRfa<br>
     * (如果链接无法点击，请将它拷贝到浏览器的地址栏中。)<br>
     * <br>
     * 人人网<br>
     * 2008年12月4日<br>
     * 
     * @param _user
     * @param _email
     * @return 验证码
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午12:01:45
     */
	private void sendRegMobilerVerifyEmail(final User _user, final String _email, String siteName)
            throws SQLException {
        this.sendRegMobilerVerifyEmail(_user, _email, null, siteName);
    }

    /**
     * @see sendRegMobilerVerifyEmail(final User _user, final String _email)
     * @param _user         用户
     * @param _email        邮箱
     * @param emailVerify   保存邮箱之后的返回结果,防止出现数据库不同步的情况
     * @throws SQLException
     */
    
    public void sendRegMobilerVerifyEmail(final User _user, final String _email, String emailVerify, String siteName)
            throws SQLException {
    	//String site_name = MyStringUtils.isNullOrEmpty(siteName)?"人人":siteName;
    	String site_name = GuideUtil.isRenrenUser(_user)?"人人":"开心";
        String ev = emailVerify;
        if (StringUtils.isEmpty(ev)) {
            ev = UserAdapter.get(_user).getEmailVerify();
        }
        StringBuffer title = new StringBuffer(""+site_name+"网注册确认");

        StringBuffer body = new StringBuffer();
        body.append(_user.getName()).append("，你好： <br />");
        body.append("       点击下面的链接即可完成邮箱验证.<br />");
        body.append("<br />");
        body.append("验证成功后，你的"+site_name+"网帐号将更改为：").append(_email).append("，手机号码仍可登录"+site_name+"网。<br />");
        body.append("<br />");
        body.append("验证码： " + ev);
        body.append("<br />");
        body.append("<br />");
        body.append("<a href='");
        body.append(Globals.urlGuide + "/mobile-activate-mail.do?id=" + _user.getId() + "&vc=" + ev);
        body.append("'>");
        body.append(Globals.urlGuide + "/mobile-activate-mail.do?id=" + _user.getId() + "&amp;vc=" + ev + "</a><br />");
        body.append("<br />");
        body.append("(如果链接无法点击，请将它拷贝到浏览器的地址栏中。)<br />");
        body.append(""+site_name+"网<br />");
        Calendar c = Calendar.getInstance();
        body.append(c.get(Calendar.YEAR)).append("年").append((c.get(Calendar.MONTH) + 1)).append(
                "月").append(c.get(Calendar.DAY_OF_MONTH)).append("日");

        Email em = new Email(_email, Globals.systemEmail, title.toString(), body.toString());
        em.setDisc(10);
        GuideLogger.printLog(" host:"+_user.getId()+" email:"+_email+" ovc:"+ev);
        
        //2010-4-15 9:46:08 区分人人开心的
        if(!GuideUtil.isRenrenUser(_user)){
        	com.kaixin.register.model.Email em_kaixin = new com.kaixin.register.model.Email(_email, Globals.systemEmail, title.toString(), body.toString());
        	em_kaixin.setDisc(10);
        	com.kaixin.register.dao.EmailDAO.getInstance().save(em_kaixin);
        }
        else{
        	EmailDAO.getInstance().save(em);
        }
    }
    
    public boolean sendMergeEmail(final User host){
    	boolean ret = true;
    	if(host == null){
    		return false;
    	}
    	String vcode = VerifyCodeDAO.getVerifyCodeByUserId(host.getId());
    	String address = host.getAccount();
   
		if(MyStringUtils.isNullOrEmpty(vcode)){
    		ret = false;
    	}
    	else{
    		try {
    			User newuser = StepProcessLogic.getInstance().getWillMergeToUser(host);
    			Date d = new Date();
    			SimpleDateFormat fmt = new SimpleDateFormat("MM月dd日");
    			String dt = fmt.format(d);
    			int disc = 10000020;
    			String title = "账号信息认证";
    			StringBuffer body = new StringBuffer();
    	        String name = newuser==null?"":newuser.getName();
    	        if(name == null){
    	        	name = "";
    	        }
    	        if(name.trim().equals("")){
    	        	name = "";
    	        }
    	        else{
    	        	name = name + "，";
    	        }
    	        
				body.append(name).append("你好： <br />");
    	        body.append("<br />");
    	        body.append("你在"+dt+"使用糯米账号登录人人网，请填写验证码，绑定人人网账号。<br />");
    	        body.append("<br />");
    	        body.append("验证码： " + vcode);
    	        body.append("<br />");
    	        body.append("<br />");
    	        body.append("<br />");
    	        
    			ProfileEmail.send(disc, address, EmailConstants.RR_OFFICIAL_FROM, title, body.toString(), "renren.com");
    			GuideLogger.printLog(" host:"+host.getId()+" email:"+address+" vcode:"+vcode+" send 1");
    		 	if(Globals.DEBUG_ENV){
    	    		address = "inshion@renren.com";
    	    		ProfileEmail.send(disc, address, EmailConstants.RR_OFFICIAL_FROM, title, body.toString(), "renren.com");
    	    		GuideLogger.printLog(" host:"+host.getId()+" email:"+address+" vcode:"+vcode+" send 2");
    	    	}
    		} catch (SQLException e) {
    			GuideLogger.printLog(" host:"+host.getId()+" email:"+address+" vcode:"+vcode+" e:"+e.toString(),GuideLogger.ERROR);
    			e.printStackTrace();
    		}
    	}
		GuideLogger.printLog(" host:"+host.getId()+" email:"+address+" vcode:"+vcode+" ret:"+ret);
		return ret;
    }
    
    public static void main(String[] args) throws SQLException {
		RegEmailLogic logic = new RegEmailLogic();
		logic.sendRegMobilerVerifyEmail(new User(), "", "");
	}
}

package com.xiaonei.reg.register.form.base;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.xiaonei.reg.register.form.base.userganerate.IBaseInfo;
import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

public abstract class BaseRegForm extends RegForm implements IBaseInfo {

	private static final long serialVersionUID = -5084882750716023415L;
	// 权限控制
	protected int basicControl;
	// 出生年月日
	protected String birth_year;
	protected String birth_day;
	protected String birth_month;

	// 性别
	protected String gender;
	// 姓名
	protected String name;
	// 密码
	protected String pwd;
	protected String repwd;

	/**
	 * 账户类型
	 */
	protected int accType;
	/**
	 * nickname id
	 */
	protected String nicknameId;
	// 邮箱
	protected String regEmail;

	// 阶段
	protected int stage;
	// 大学
	protected String typeOfCourse;
	protected int universityid;
	protected String universityname;
	// 工作
	protected String workplaceid;
	protected String workplacename;
	// 高中 技校 初中
	protected String schoolType;
	protected int enrollYear;
	protected String highSchoolCode;
	protected String highSchoolName;
	protected String highSchoolClass;
	protected String juniorHighSchoolName;
	protected String juniorHighSchoolId;
	protected String techHighSchoolName;
	protected String techHighSchoolId;
	// 所在地
	protected String homecitycode;
	protected String homecityname;
	protected String homeprovince;

	// 头像路径
	protected String tinyUrl;
	protected String mainUrl;
	protected String largeUrl;
	protected String headUrl;
	protected String porid;
	// ------------------------------------------
	// 邀请相关
	// 邀请人id
	protected int inviterId;
	protected int inviteType;
	protected String inviteeEmail;
	protected String groupName;
	// app
	protected int appId;
	protected String appToken;
	protected String appName;
	// --------------------------------------------
	// 来源URL 统计用
	protected String fromUrl;
	// 链接类型
	protected String code;
	protected String from;
	// 市场渠道活动
	protected String channel;
	protected String iu;
	protected String ic;
	// ------------------------------------------
	// 审核
	protected String audit;
	protected String department;
	protected String dormitory;
	protected String studentnumber;

	// 新版邀请，统一成一个参数
	protected String uuid;
	/** 联盟统计g值 */
	protected String g;
	/** 联盟统计b值 */
	protected String b;

	protected String action_id;
	protected String ss;

	//page id
	protected int pageId;
	
	public String trimLine(String str) {
        Pattern p = Pattern.compile("\t|\r|\n");
        Matcher m = p.matcher(str);
        String after = m.replaceAll("");
        return after;
    }
	
	public String toString() {
		return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE)
				.append("basicControl", basicControl).append("birth_year",
						birth_year).append("birth_day", birth_day).append(
						"birth_month", birth_month).append("gender", gender)
				.append("name", name).append("pwd", pwd).append("repwd", repwd)
				.append("accType", accType).append("nicknameId", nicknameId)
				.append("regEmail", regEmail).append("stage", stage).append(
						"typeOfCourse", typeOfCourse).append("universityid",
						universityid).append("universityname", universityname)
				.append("workplaceid", workplaceid).append("workplacename",
						workplacename).append("schoolType", schoolType).append(
						"enrollYear", enrollYear).append("highSchoolCode",
						highSchoolCode)
				.append("highSchoolName", highSchoolName).append(
						"highSchoolClass", highSchoolClass).append(
						"juniorHighSchoolName", juniorHighSchoolName).append(
						"juniorHighSchoolId", juniorHighSchoolId).append(
						"techHighSchoolName", techHighSchoolName).append(
						"techHighSchoolId", techHighSchoolId).append(
						"homecitycode", homecitycode).append("homecityname",
						homecityname).append("homeprovince", homeprovince)
				.append("tinyUrl", tinyUrl).append("mainUrl", mainUrl).append(
						"largeUrl", largeUrl).append("headUrl", headUrl)
				.append("porid", porid).append("inviterId", inviterId).append(
						"inviteType", inviteType).append("inviteeEmail",
						inviteeEmail).append("groupName", groupName).append(
						"appId", appId).append("appToken", appToken).append(
						"fromUrl", fromUrl).append("code", code).append("from",
						from).append("channel", channel).append("iu", iu)
				.append("ic", ic).append("audit", audit).append("department",
						department).append("dormitory", dormitory).append(
						"studentnumber", studentnumber).append("uuid", uuid)
				.append("g", g).append("b", b).append("action_id", action_id)
				.append("ss", ss).append("pageId", pageId).toString();
	}

	final public String getChannel() {
		return this.channel;
	}

	final public String getCode() {
		return this.code;
	}

	final public String getFrom() {
		return this.from;
	}

	final public String getFromUrl() {
		return this.fromUrl;
	}

	final public String getIc() {
		return this.ic;
	}

	final public String getIu() {
		return this.iu;
	}

	final public String getUuid() {
		return this.uuid;
	}

	final public void setChannel(String channel) {
		this.channel = channel;
	}

	final public void setCode(String code) {
		this.code = code;
	}

	final public void setFrom(String from) {
		this.from = from;
	}

	final public void setFromUrl(String fromUrl) {
		this.fromUrl = fromUrl;
	}

	final public void setIc(String ic) {
		this.ic = ic;
	}

	final public void setIu(String iu) {
		this.iu = iu;
	}

	final public void setUuid(String uuid) {
		this.uuid = uuid;
	}

	public void setG(String g) {
		this.g = g;
	}

	public String getG() {
		return g;
	}

	public void setB(String b) {
		this.b = b;
	}

	public String getB() {
		return b;
	}

	public void setAction_id(String action_id) {
		this.action_id = action_id;
	}

	public String getAction_id() {
		return action_id;
	}

	public void setSs(String ss) {
		this.ss = ss;
	}

	public String getSs() {
		return ss;
	}
    
    public int getPageId() {
        return pageId;
    }
    
    public void setPageId(int pageId) {
        this.pageId = pageId;
    }    
}

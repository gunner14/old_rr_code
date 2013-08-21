package com.xiaonei.page.xoa.domain;


public class UserView {
	
	private int id;
	private String account;
	private String name;
	private String nickName;
	private String domain;
    /**
     * 获取用户帐号当前状态 0:激活；3:未激活；6:封禁；7:自杀
     */
	private int status;
	private int auth;
	private int rights;
	private int star;
	private int level;
	private int loginCount;
	private String headUrl;
	private String largeUrl;
	private String mainUrl;
	private String tinyUrl;
	private String headFullUrl;
	private String largeFullUrl;
	private String mainFullUrl;
	private String tinyFullUrl;
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getAccount() {
		return account;
	}
	public void setAccount(String account) {
		this.account = account;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getNickName() {
		return nickName;
	}
	public void setNickName(String nickName) {
		this.nickName = nickName;
	}
	public String getDomain() {
		return domain;
	}
	public void setDomain(String domain) {
		this.domain = domain;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public int getAuth() {
		return auth;
	}
	public void setAuth(int auth) {
		this.auth = auth;
	}
	public int getRights() {
		return rights;
	}
	public void setRights(int rights) {
		this.rights = rights;
	}
	public int getStar() {
		return star;
	}
	public void setStar(int star) {
		this.star = star;
	}
	public int getLevel() {
		return level;
	}
	public void setLevel(int level) {
		this.level = level;
	}
	public int getLoginCount() {
		return loginCount;
	}
	public void setLoginCount(int loginCount) {
		this.loginCount = loginCount;
	}
	public String getHeadUrl() {
		return headUrl;
	}
	public void setHeadUrl(String headUrl) {
		this.headUrl = headUrl;
	}
	public String getLargeUrl() {
		return largeUrl;
	}
	public void setLargeUrl(String largeUrl) {
		this.largeUrl = largeUrl;
	}
	public String getMainUrl() {
		return mainUrl;
	}
	public void setMainUrl(String mainUrl) {
		this.mainUrl = mainUrl;
	}
	public String getTinyUrl() {
		return tinyUrl;
	}
	public void setTinyUrl(String tinyUrl) {
		this.tinyUrl = tinyUrl;
	}
	public String getHeadFullUrl() {
		return headFullUrl;
	}
	public void setHeadFullUrl(String headFullUrl) {
		this.headFullUrl = headFullUrl;
	}
	public String getLargeFullUrl() {
		return largeFullUrl;
	}
	public void setLargeFullUrl(String largeFullUrl) {
		this.largeFullUrl = largeFullUrl;
	}
	public String getMainFullUrl() {
		return mainFullUrl;
	}
	public void setMainFullUrl(String mainFullUrl) {
		this.mainFullUrl = mainFullUrl;
	}
	public String getTinyFullUrl() {
		return tinyFullUrl;
	}
	public void setTinyFullUrl(String tinyFullUrl) {
		this.tinyFullUrl = tinyFullUrl;
	}
	
	
}

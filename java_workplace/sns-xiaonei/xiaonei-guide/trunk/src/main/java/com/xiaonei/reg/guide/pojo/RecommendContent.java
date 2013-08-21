package com.xiaonei.reg.guide.pojo;

import java.sql.Timestamp;

import com.renren.sns.minisite.xoa.model.SiteInfo;


public class RecommendContent {

    private int id;
    private long content_business_id;
    private int content_type;
    private int hits;
    private int show_count;
    private int user_keep_rate;
    private Timestamp op_time;
    private SiteInfo siteInfo;
    
    private Float ep;
    
    private String nickname;
    
    private String interest;
    
    
    public int getId() {
        return id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public long getContent_business_id() {
        return content_business_id;
    }
    
    public void setContent_business_id(long content_business_id) {
        this.content_business_id = content_business_id;
    }
    
    public int getContent_type() {
        return content_type;
    }
    
    public void setContent_type(int content_type) {
        this.content_type = content_type;
    }
    
    public int getHits() {
        return hits;
    }
    
    public void setHits(int hits) {
        this.hits = hits;
    }
    
    public int getShow_count() {
        return show_count;
    }
    
    public void setShow_count(int show_count) {
        this.show_count = show_count;
    }
    
    public int getUser_keep_rate() {
        return user_keep_rate;
    }
    
    public void setUser_keep_rate(int user_keep_rate) {
        this.user_keep_rate = user_keep_rate;
    }
    
    public Timestamp getOp_time() {
        return op_time;
    }
    
    public void setOp_time(Timestamp op_time) {
        this.op_time = op_time;
    }

    
    public SiteInfo getSiteInfo() {
        return siteInfo;
    }

    
    public void setSiteInfo(SiteInfo siteInfo) {
        this.siteInfo = siteInfo;
    }

    
    public Float getEp() {
        return ep;
    }

    
    public void setEp(Float ep) {
        this.ep = ep;
    }

	public String getNickname() {
		return nickname;
	}

	public void setNickname(String nickname) {
		this.nickname = nickname;
	}

	public String getInterest() {
		return interest;
	}

	public void setInterest(String interest) {
		this.interest = interest;
	}
    
    
}

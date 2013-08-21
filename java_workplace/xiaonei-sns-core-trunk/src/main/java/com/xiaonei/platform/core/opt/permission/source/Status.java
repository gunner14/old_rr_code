package com.xiaonei.platform.core.opt.permission.source;

import java.io.Serializable;

import mop.hi.oce.domain.network.NetworkBigObjectN;

import com.xiaonei.platform.core.opt.permission.AbsSource;

public class Status extends AbsSource implements Serializable{
    
    private static final long serialVersionUID = -2642497006017304217L;
    
    private int stage ;
    private int net ;
    private int auth ;
    private int status ;
    private int selected ;
    private int fromToPath=-1000 ;
    private NetworkBigObjectN netbig;
    
    /**
     * @param id {@link Long} 可以写成用户ID
     * @param userId {@link Integer} 用户ID {@link com.xiaonei.platform.core.model.User#getId()}
     * @param stage {@link Integer} 用户的阶段 {@link com.xiaonei.platform.core.model.User#getStage()}
     * @param net {@link Integer} 用户的网络 {@link com.xiaonei.platform.core.model.User#getUniv()}
     * @param auth {@link Integer} 权限 {@link com.xiaonei.platform.core.model.User#getAuth()}
     * @param status {@link Integer} 用户当前状态 {@link com.xiaonei.platform.core.model.User#getStatus()}
     * @param control {@link Integer} 两人之间的关系, {@link com.xiaonei.platform.core.model.UserConfigInfo#getBasicConfig()}
     *
     * @author tai.wang@opi-corp.com Aug 5, 2010 - 1:34:03 PM
     */
    public Status(long id,int userId,int stage,int net,int auth,int status,int control){
        super(id,userId,control) ;
        this.stage=stage;
        this.net = net ;
        this.auth=auth;
        this.status=status ;

    }

    /**
     * @param id {@link Integer} 可以写成用户ID
     * @param userId {@link Integer} 用户ID {@link com.xiaonei.platform.core.model.User#getId()}
     * @param stage {@link Integer} 用户的阶段 {@link com.xiaonei.platform.core.model.User#getStage()}
     * @param net {@link Integer} 用户的网络 {@link com.xiaonei.platform.core.model.User#getUniv()}
     * @param auth {@link Integer} 权限 {@link com.xiaonei.platform.core.model.User#getAuth()}
     * @param status {@link Integer} 用户当前状态 {@link com.xiaonei.platform.core.model.User#getStatus()}
     * @param control {@link Integer} 两人之间的关系, {@link com.xiaonei.platform.core.model.UserConfigInfo#getBasicConfig()}
     *
     * @author tai.wang@opi-corp.com Aug 5, 2010 - 1:34:03 PM
     */
    public Status(int id,int userId,int stage,int net,int auth,int status,int control){
        super(id,userId,control) ;
        this.stage=stage;
        this.net = net ;
        this.auth=auth;
        this.status=status ;

    }
	public int getAuth() {
		return auth;
	}
	public void setAuth(int auth) {
		this.auth = auth;
	}
	public int getNet() {
		return net;
	}
	public void setNet(int net) {
		this.net = net;
	}
	public int getStage() {
		return stage;
	}
	public void setStage(int stage) {
		this.stage = stage;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	@Override
	public String toString() {
		String tmp = "----------- 这个Status对象的字段值：\n";
		tmp += "-- id="+super.getId()+"\n";
		tmp += "-- userId="+super.getUserId()+"\n";
		tmp += "-- stage="+stage+"\n";
		tmp += "-- net="+net+"\n";
		tmp += "-- auth="+auth+"\n";
		tmp += "-- status="+status+"\n";
		tmp += "-- control="+super.getControl()+"\n";
		tmp += "-- path="+fromToPath+"\n";
		tmp += "-----------------------------------\n";
		return tmp;
	}
	public int getFromToPath() {
		return fromToPath;
	}
	public void setFromToPath(int fromToPath) {
		this.fromToPath = fromToPath;
	}
	public int getSelected() {
		return selected;
	}
	public void setSelected(int selected) {
		this.selected = selected;
	}

	/**
	 * @see NetworkBigObjectN
	 * 每个List都只要name和id
	 * 
	 * @param netbig the netbig to set
	 */
	public void setNetbig(NetworkBigObjectN netbig) {
		this.netbig = netbig;
		
	}

	/**
	 * @see NetworkBigObjectN
	 * 
	 * @return the netbig
	 */
	public NetworkBigObjectN getNetbig() {
		return netbig;
	}
	
		
}

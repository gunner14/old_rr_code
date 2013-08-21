package com.xiaonei.platform.core.model;

import java.io.Serializable;

import xce.userbase.CStateMOBILEBIND;
import xce.userbase.CStateSELECTED;
import xce.userbase.CStateV6;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.platform.core.opt.network.NetworkManager;

/**
 * 
 * 对OCE的UserCache再封装
 * 
 */
public class WUserCache extends com.xiaonei.xce.usercache.UserCache implements
Serializable {
    private static final long serialVersionUID = 1L;

    public WUserCache() {

    }

    public String getTiny_Url() {
        return HeadUrlUtil.getHeadFullUrl(super.getTinyUrl());
    }

    //	public String getHead_Url() {
    //		return HeadUrlUtil.getHeadFullUrl(super.getHeadUrl());
    //	}

    public int getUnivId() {
        return super.getUniv();
    }

    public String getUnivName() {
        return NetworkManager.getInstance().getNetById(super.getUniv())
                .getName();
    }

    /**
     * <ul>
     * <li>手机是否在线，统一使用这里的，代替userstatus</li>
     * </ul>
     * 
     * @return
     */
    public boolean isMobileBind() {
        return 0 != (this.getState() & (1 << CStateMOBILEBIND.value));
    }

    /**
     * 判断是否为星级用户
     * 
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Mar 19, 2012 5:53:52 PM
     * 
     * @return
     */
    public boolean isStarUser() {
        return 0 != (this.getState() & (1 << CStateSELECTED.value));
    }

    public boolean isV6User() {
        return 0 != (this.getState() & (1 << CStateV6.value));
    }

}
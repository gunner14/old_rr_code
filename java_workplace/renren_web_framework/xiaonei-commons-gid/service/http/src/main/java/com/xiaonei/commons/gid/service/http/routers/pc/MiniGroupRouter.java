package com.xiaonei.commons.gid.service.http.routers.pc;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.commons.gid.service.http.routers.Router;
import com.xiaonei.commons.gid.util.G;

/**
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class MiniGroupRouter implements Router {

    @Override
    public String getTarget(long gid, HttpServletRequest request) {
        return "http://qun.renren.com/qun/ugc/home/" + G.extractX(gid);
    }

}

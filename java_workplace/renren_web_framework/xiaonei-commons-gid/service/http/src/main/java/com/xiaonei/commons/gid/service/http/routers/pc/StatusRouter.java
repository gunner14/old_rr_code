package com.xiaonei.commons.gid.service.http.routers.pc;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.model.Entry;
import com.renren.entries.xoa.api.EntryService;
import com.xiaonei.commons.gid.service.http.routers.Router;
import com.xiaonei.commons.gid.util.G;

/**
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
//TODO: 小站、公共主页的状态、日志、分享、视频地址和全站的地址是不一样的
public class StatusRouter implements Router {

    private EntryService entryService;

    @Autowired
    public void setEntryService(EntryService entryService) {
        this.entryService = entryService;
    }

    @Override
    public String getTarget(long gid, HttpServletRequest request) {
        String author = request.getParameter("u");
        Entry entry = entryService.get(gid).submit().awaitUninterruptibly().getContent();
        if (entry == null) {
            if (author == null) {
                return "@not found entry " + gid;
            }
        } else {
            String field = entry.getFieldValue("author");
            if (field == null) {
                if (author == null) {
                    return "@not found author for entry " + gid;
                }
            } else {
                author = field;
            }
        }
        long statusId = G.extractX(gid);
        return "http://status.renren.com/status/?id=" + author + "&doingId=" + statusId;
    }

}

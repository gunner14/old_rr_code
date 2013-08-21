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
public class PhotoRouter implements Router {

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
        return "http://photo.renren.com/photo/" + author + "/photo-" + G.extractX(gid);
    }

}

package com.renren.entries.xoa.controllers;

import net.paoding.rose.web.annotation.DefValue;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.core.EntryService;
import com.renren.entries.model.Tag;
import com.xiaonei.commons.gid.util.Type;

/**
 * 对外提供的xoa接口
 * 
 * @author qieqie.wang@gmail.com
 */
public class TagController {

    @Autowired
    private EntryService entryService;

    @Get("{id:\\d+}")
    public Tag get(@Param("id") int id) {
        return entryService.getTag(id);
    }

    // get?type=xxx&value=yyy&create=true|false
    @Get("/get")
    public Tag get(@Param("type") Type type, @Param("value") String value,
            @Param("create") @DefValue("false") boolean create) {
        String namespace = "/";
        if (type != null) {
            if (type.getValue() >= 0x300) {
                throw new IllegalArgumentException("wrong namespace " + type);
            }
            namespace = "/" + type.toString().toLowerCase();
        }
        return entryService.getTag(namespace, value, create);
    }

}

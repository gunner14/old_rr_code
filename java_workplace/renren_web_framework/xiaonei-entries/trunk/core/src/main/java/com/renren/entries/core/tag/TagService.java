package com.renren.entries.core.tag;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataIntegrityViolationException;

import com.renren.entries.core.dao.TagDAO;
import com.renren.entries.model.Tag;

/**
 * Tag相关的服务逻辑
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-9 下午10:31:12
 */
public class TagService {

    protected Log logger = LogFactory.getLog(this.getClass());

    @Autowired
    private TagDAO tagDAO;

    public Tag getTag(int id) {
        return tagDAO.getById(id);
    }

    /**
     * 
     * @param ids
     * @return
     */
    public Tag[] getTags(int[] ids) {
        Tag[] tags = new Tag[ids.length];
        for (int i = 0; i < tags.length; i++) {
            tags[i] = getTag(ids[i]);
        }
        return tags;
    }

    /**
     * 给定tag，获取其ID
     * 
     * @param value
     * @return
     */
    public Tag getTag(String namespace, String value, boolean create) {
        namespace = checkNamespace(namespace);
        int error = 0;
        DataIntegrityViolationException exception = null;
        while (error < 3) {
            Tag tag = tagDAO.getByValue(namespace, value);
            if (tag == null && create) {
                try {
                    tagDAO.save(namespace, value);
                } catch (DataIntegrityViolationException ex) {
                    error++;
                    exception = ex;
                    try {
                        Thread.sleep(10);
                    } catch (Exception e) {}
                }
            } else {
                return tag;
            }
        }
        throw exception;
    }

    /**
     * 
     * @param values
     * @return
     */
    public Tag[] getTags(String namespace, String[] values, boolean create) {
        namespace = checkNamespace(namespace);
        Tag[] tags = new Tag[values.length];
        for (int i = 0; i < values.length; i++) {
            tags[i] = getTag(namespace, values[i], create);
        }
        return tags;
    }

    private String checkNamespace(String namespace) {
        if (namespace == null) {
            return "/";
        }
        namespace = namespace.trim().toLowerCase();
        if (!namespace.startsWith("/")) {
            namespace = "/" + namespace;
        }
        while (namespace.length() > 1 && namespace.endsWith("/")) {
            namespace = namespace.substring(0, namespace.length() - 1);
        }
        return namespace;
    }

}

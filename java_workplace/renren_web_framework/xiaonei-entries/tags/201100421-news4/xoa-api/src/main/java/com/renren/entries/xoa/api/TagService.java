package com.renren.entries.xoa.api;

import com.renren.entries.model.Tag;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaParam;
import com.renren.xoa.lite.annotation.XoaPost;
import com.renren.xoa.lite.annotation.XoaService;
import com.xiaonei.commons.gid.util.Type;

/**
 */
@XoaService(serviceId = "entries.xoa.renren.com")
public interface TagService {

    /**
     * 根据指定id获取指定tag
     * 
     * @param id
     * @return
     */
    @XoaGet("/tag/{id}")
    public ServiceFuture<Tag> get(@XoaParam("id") int id);

    /**
     * 根据一组id批量查tag
     * 
     * @param ids
     * @return
     */
    @XoaGet("/tag")
    public ServiceFuture<Tag> gets(@XoaParam("id") int[] ids);
    
    /**
     * 
     * @param type master类型，只能是 null(null代表全站范围的)、{@link Type#PERSON}、
     *        {@link Type#XIAOZHAN}、 {@link Type#PAGE}、 {@link Type#XIAOZU}
     *        、 {@link Type#MINI_GROUP}等这类的类型，请不要传入其他类型的
     * @param tagValue
     * @param create 如果给定的标签还不存在，是否要自动创建？
     * @return
     */
    @XoaGet("/tag/get")
    public ServiceFuture<Tag> get(@XoaParam("type") Type type, @XoaParam("value") String tagValue,
            @XoaParam("create") boolean create);
    
    /**
     * 增加指定tag的被tag计数
     * @param id
     * @param count
     * @return
     */
    @XoaPost("/tag/{id}/count/increament")
    public ServiceFuture<Tag> increaseTagCount(@XoaParam("id") int id, @XoaParam("count") int count);
}

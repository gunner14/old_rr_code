package com.xiaonei.commons.gid.util;

import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Map;

/**
 * 统一ID子类型定义及其枚举。
 * <p>
 * {@link SubType}定义了各个子类型体的类型，以及提供了从整型值或字符串转化为 {@link SubType}实例的方法
 * 
 * @author 王志亮 zhiliang.wang@renren-inc.com
 * 
 */
public class SubType {

    //-----------------------------------------------------------------------
    // 保存所有的SubType实例，并使SubType唯一
    private static Map<Long, SubType> ALL = new HashMap<Long, SubType>();

    //-----------------------------------------------------------------------

    /** 个人 */
    public static final SubType PERSON = new SubType(Type.PERSON.value);

    /** 公共主页 */
    public static final SubType PAGE = new SubType(Type.PAGE.value);

    /** 小站 */
    public static final SubType XIAOZHAN = new SubType(Type.XIAOZHAN.value);

    /** 小组 */
    public static final SubType XIAOZU = new SubType(Type.XIAOZU.value);

    /** 老群 */
    public static final SubType GROUP = new SubType(Type.GROUP.value);

    /** 私群 */
    public static final SubType MINI_GROUP = new SubType(Type.MINI_GROUP.value);

    /** 活动 */
    public static final SubType EVENT = new SubType(Type.EVENT.value);

    /** 第三方应用 */
    public static final SubType APPLICATION = new SubType(Type.APPLICATION.value);

    /** 俱乐部 */
    public static final SubType ORG = new SubType(Type.ORG.value);

    /** 评论 */
    public static final SubType COMMENT = new SubType(Type.COMMENT.value);

    /** 状态 */
    public static final SubType STATUS = new SubType(Type.STATUS.value);

    /** 日志 */
    public static final SubType BLOG = new SubType(Type.BLOG.value);

    /** 照片 */
    public static final SubType PHOTO = new SubType(Type.PHOTO.value);

    /** 视频 */
    public static final SubType VIDEO = new SubType(Type.VIDEO.value);

    /** 专辑/相册 */
    public static final SubType ALBUM = new SubType(Type.ALBUM.value);

    /** 分享 */
    public static final SubType SHARE = new SubType(Type.SHARE.value);

    /** 投票 */
    public static final SubType VOTE = new SubType(Type.VOTE.value);

    /** 留言 */
    public static final SubType GOSSIP = new SubType(Type.GOSSIP.value);

    /** 站内消息 */
    public static final SubType MESSAGE = new SubType(Type.MESSAGE.value);

    /** 请求 */
    public static final SubType REQUEST = new SubType(Type.REQUEST.value);

    /** 标签 */
    public static final SubType TAG = new SubType(Type.TAG.value);

    /** 地点 */
    public static final SubType PLACE = new SubType(Type.PLACE.value);

    /** 信息存储中心系统的entry_index条目 */
    public static final SubType ENTRY_INDEX = new SubType(Type.ENTRY_INDEX.value);

    /** 链接 */
    public static final SubType LINK = new SubType(Type.LINK.value);

    /** 音频 */
    public static final SubType AUDIO = new SubType(Type.AUDIO.value);

    /** 日志模板(九宫格、凡客体之类的模板) */
    public static final SubType BLOG_TEMPLATE = new SubType(Type.BLOG_TEMPLATE.value);

    // 二级类型(不作为一级类型的)
    static final SubType SUBTYPE_SART = new SubType(0x7FF0000);

    /** 九宫格模板类型 */
    public static final SubType BLOG_TEMPLATE_BOX = new SubType(0x7FF0001);

    //------------------------------

    // 类型到其名称的映射
    private static Map<SubType, String> TYPE_TO_STRING = new HashMap<SubType, String>();

    // 名称到器类型的映射
    private static Map<String, SubType> STRING_TO_TYPE = new HashMap<String, SubType>();

    /**
     * 通过值定义解出其类型
     * 
     * @param value
     * @return
     */
    public static SubType toType(long value) {
        SubType type = SubType.ALL.get(value);
        if (type == null) {
            throw new IllegalArgumentException("wrong type value '0x"
                    + Long.toHexString(value).toUpperCase() + "'");
        }
        return type;
    }

    /**
     * 通过名称解出其类型
     * 
     * @param typeName
     * @return
     */
    public static SubType toType(String typeName) {
        typeName = typeName.toUpperCase();
        if (typeName.startsWith("0x") || typeName.startsWith("0X")) {
            return toType(Long.valueOf(typeName.substring(2)));
        } else {
            resolveStrings();
            SubType type = (SubType) STRING_TO_TYPE.get(typeName);
            if (type == null) {
                throw new IllegalArgumentException("wrong type name '" + typeName + "'");
            }
            return type;
        }
    }

    //--------------------------------------------------------------------------

    /** 类型定义值 */
    public final long value;

    /**
     * 构造一个给定值的类型
     * 
     * @param value
     * @throws IllegalArgumentException 当创建重复的GType时
     */
    private SubType(long value) {
        if (ALL.containsKey(value)) {
            throw new IllegalArgumentException("gtype replicated " + value);
        }
        this.value = value;
        ALL.put(value, this);
    }

    public long getValue() {
        return value;
    }

    /**
     * 
     */
    @Override
    public boolean equals(Object obj) {
        return obj == this;
    }

    /**
     * 
     */
    @Override
    public int hashCode() {
        return (int) value;
    }

    /**
     * 返回其类型名称
     */
    @Override
    public String toString() {
        resolveStrings();
        return TYPE_TO_STRING.get(this);
    }

    /**
     * 确认 {@link #TYPE_TO_STRING}以及 {@link #STRING_TO_TYPE}两个参数就绪
     */
    private static void resolveStrings() throws Error {
        if (TYPE_TO_STRING.size() == 0) {
            synchronized (TYPE_TO_STRING) {
                try {
                    if (TYPE_TO_STRING.size() == 0) {
                        for (Field f : SubType.class.getFields()) {
                            if (f.getType() == SubType.class) {
                                SubType type = (SubType) f.get(null);
                                TYPE_TO_STRING.put(type, f.getName());
                                STRING_TO_TYPE.put(f.getName(), type);
                            }
                        }
                    }
                } catch (Exception e) {
                    TYPE_TO_STRING.clear();
                    STRING_TO_TYPE.clear();
                    throw new Error(e);
                }
            }
        }
    }
}

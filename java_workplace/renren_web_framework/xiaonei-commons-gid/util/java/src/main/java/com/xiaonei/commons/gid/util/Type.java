package com.xiaonei.commons.gid.util;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 统一ID类型定义及其枚举。
 * <p>
 * {@link Type}定义了各个类型体的类型，以及提供了从整型值或字符串转化为 {@link Type}实例的方法
 * 
 * @author 王志亮 zhiliang.wang@renren-inc.com
 * 
 */
public class Type {

    //-----------------------------------------------------------------------
    // 保存所有的Type实例，并使Type唯一
    private static Map<Long, Type> ALL = new HashMap<Long, Type>();

    //-----------------------------------------------------------------------

    /** 个人 */
    public static final Type PERSON = new Type(0x100);

    /** 公共主页 */
    public static final Type PAGE = new Type(0x110);

    /** 小站 */
    public static final Type XIAOZHAN = new Type(0x120);

    /** 小组 */
    public static final Type XIAOZU = new Type(0x130);

    /** 老群 */
    public static final Type GROUP = new Type(0x140);

    /** 私群 */
    public static final Type MINI_GROUP = new Type(0x150);

    /** 活动 */
    public static final Type EVENT = new Type(0x160);

    /** 第三方应用 */
    public static final Type APPLICATION = new Type(0x170);

    /** 俱乐部 */
    public static final Type ORG = new Type(0x180);
    
    /** 家庭 */
    public static final Type JIA = new Type(0x190);

    /** 评论 */
    public static final Type COMMENT = new Type(0x300);

    /** 状态 */
    public static final Type STATUS = new Type(0x310);

    /** 日志 */
    public static final Type BLOG = new Type(0x320);

    /** 照片 */
    public static final Type PHOTO = new Type(0x330);

    /** 视频 */
    public static final Type VIDEO = new Type(0x340);

    /** 专辑/相册 */
    public static final Type ALBUM = new Type(0x350);

    /** 分享 */
    public static final Type SHARE = new Type(0x360);

    /** 投票 */
    public static final Type VOTE = new Type(0x370);

    /** 留言 */
    public static final Type GOSSIP = new Type(0x380);

    /** 站内消息 */
    public static final Type MESSAGE = new Type(0x390);

    /** 请求 */
    public static final Type REQUEST = new Type(0x3A0);

    /** 标签 */
    public static final Type TAG = new Type(0x3B0);

    /** 地点 */
    public static final Type PLACE = new Type(0x3C0);

    /** 信息存储中心系统的entry_index条目 */
    public static final Type ENTRY_INDEX = new Type(0x3D0);

    /** 链接 */
    public static final Type LINK = new Type(0x3E0);

    /** 音频 */
    public static final Type AUDIO = new Type(0x3F0);

    /** 日志模板(九宫格、凡客体之类的模板) */
    public static final Type BLOG_TEMPLATE = new Type(0x3A1);

    // 二级类型(不作为一级类型的)
    static final Type SUBTYPE_SART = new Type(0x7FF000);

    //------------------------------

    // 类型到其名称的映射
    private static Map<Type, String> TYPE_TO_STRING = new HashMap<Type, String>();

    // 名称到器类型的映射
    private static Map<String, Type> STRING_TO_TYPE = new HashMap<String, Type>();

    /**
     * 返回所有的类型，按照类型的字母顺序
     * 
     * @return
     */
    public static List<Type> allTypes() {
        ArrayList<Type> allTypes = new ArrayList<Type>();
        resolveStrings();
        allTypes.addAll(TYPE_TO_STRING.keySet());
        Collections.sort(allTypes, new Comparator<Type>() {

            @Override
            public int compare(Type o1, Type o2) {
                return o1.getName().compareTo(o2.getName());
            }
        });
        return allTypes;
    }

    /**
     * 通过值定义解出其类型
     * 
     * @param value
     * @return
     */
    public static Type toType(long value) {
        Type type = Type.ALL.get(value);
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
    public static Type toType(String typeName) {
        typeName = typeName.toUpperCase();
        if (typeName.startsWith("0x") || typeName.startsWith("0X")) {
            return toType(Long.valueOf(typeName.substring(2)));
        } else {
            resolveStrings();
            Type type = (Type) STRING_TO_TYPE.get(typeName);
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
    private Type(long value) {
        if (ALL.containsKey(value)) {
            throw new IllegalArgumentException("gtype replicated " + value);
        }
        this.value = value;
        ALL.put(value, this);
    }

    /**
     * 
     * @return
     */
    public long getValue() {
        return value;
    }

    /**
     * 返回类型名称
     * 
     * @return
     */
    public String getName() {
        return toString();
    }

    /**
     * 返回“类型－原ID”字符串
     * 
     * @param x
     * @return
     */
    public String toString(long x) {
        return getName() + "-" + x;
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
                        for (Field f : Type.class.getFields()) {
                            if (f.getType() == Type.class) {
                                Type type = (Type) f.get(null);
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

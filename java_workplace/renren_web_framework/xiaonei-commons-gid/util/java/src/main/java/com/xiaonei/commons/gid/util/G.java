package com.xiaonei.commons.gid.util;

/**
 * {@link G} 提供了将原ID转化为统一ID及其相关的算法。
 * <p>
 * 统一ID的转化算法由架构组和数据中心联合统一提供，如果统一ID的算法尚未包括您的业务类型，请email给 <a
 * href="mailto:xn.arch@opi-corp.com">架构组</a>和<a
 * href="mailto:xn_db@opi-corp.com">数据中心</a>，他们会为您服务
 * 
 * @see <a
 *      href="http://doc.d.xiaonei.com/index.php?title=%E7%BB%9F%E4%B8%80ID%E7%9A%84%E8%AE%BE%E8%AE%A1">统一ID的设计</a>
 * 
 * @author 王志亮 zhiliang.wang@renren-inc.com
 * @author 张铁蕾 tielei.zhang@renren-inc.com
 * @author 刘启荣 qirong.liu@renren-inc.com
 * @author 冷昊 hao.leng@renren-inc.com
 * @version 统一ID算法1.4
 * 
 */
public class G {

    /**
     * 类型信息要左移多少位，也就是说原ID再GID中占去了多少位
     */
    private static final int TYPE_SHIFT = 52;
    
    /**
     * 将原ID转化为统一ID。
     * 
     * @see #id(long, Type)
     * @param type
     * @param x
     * @return
     */
    public static long id(Type type, long x) {
        return id(x, type);
    }

    /**
     * 将原ID转化为统一ID。
     * <p>
     * 有一部分类型的统一ID和其原ID相同，有一部分类型的统一ID是通过加前缀加工出来的(二进制)
     * 
     * @see <a
     *      href="http://doc.d.xiaonei.com/index.php?title=%E7%BB%9F%E4%B8%80ID%E7%9A%84%E8%AE%BE%E8%AE%A1">统一ID的设计</a>
     * 
     * @param x 原ID
     * @param type 原ID的类型
     * @return 该原Id对应的统一ID
     * @throws IllegalArgumentException 非法原ID或非法type时
     */
    public static long id(long x, Type type) {
        // 不能为零或负数
        if (x <= 0) {
            throw new IllegalArgumentException("wrong x(" + x + "), positive only");
        }
        // type为个人、Page、俱乐部、小站、家庭、私群原ID就是统一ID
		if (type.value < 0x300) {
			if (type == Type.PERSON || type == Type.PAGE || type == Type.ORG
					|| type == Type.XIAOZHAN || type == Type.JIA
					|| type == Type.MINI_GROUP) {
				if (!isTypeOf(x, type)) {
					throw new IllegalArgumentException("x(" + x
							+ ") is not a type of " + type);
				}
				return x;
			}
		}
        // 不能超过44位，只能占用43位
        if (x > upperXValueOf(type)) {
            throw new IllegalArgumentException("x(" + x + ") is not a type of " + type
                    + ", max value is " + upperXValueOf(type));
        }
        // 剩下的需要对原ID做一定的操作
        return (type.value << TYPE_SHIFT) | x;
    }

    /**
     * 所给的统一ID是否是该类型的？
     * 
     * @param expectedType
     * @param gid
     * @return
     */
    public static boolean isTypeOf(long gid, Type expectedType) {
    	Type realType = extractType(gid);
    	
    	//FIXME 由于俱乐部和Page要合并，所以这里认为ORG类型的也是PAGE类型
    	//现在的做法比较恶心，将来要去掉ORG这个类型
    	if (expectedType == Type.PAGE) {
    		return realType == Type.PAGE || realType == Type.ORG;
    	}
        return expectedType == realType;
    }

    /**
     * 从统一ID中提取出其原ID
     * 
     * @param gid
     * @return
     */
    public static long extractX(long gid) {
        return gid & 0xFFFFFFFFFFFL; // 11个F=44位
    }

    /**
     * ugc类型的最大原ID规定
     * 
     * @param type
     * @return
     */
    public static long upperXValueOf(Type type) {
        return 0xFFFFFFFFFFFL;
    }

    /**
     * 从统一ID中提取其类型
     * 
     * @see http://wiki.rrdba.cn/wiki/index.php/Id_seq
     * @param gid
     * @return 如果找不到gid对应的类型返回null
     */
    public static Type extractType(long gid) {
        if (gid <= 0) {
            return null;
        }
        if ((gid >= 1 && gid <= 599999999) || (gid >= 700000000 && gid <= 2147483647)) {
            return Type.PERSON;
        }
        if (gid >= 670000000 && gid <= 670999999) {
            return Type.ORG;
        }
        if (gid >= 671000000 && gid <= 672999999) {
            return Type.XIAOZHAN;
        }
        if (gid >= 673000000 && gid <= 673999999) {
            return Type.JIA;
        }
        if (gid >= 680000000 && gid <= 680999999) {
            return Type.MINI_GROUP;
        }
        if (gid >= 600000000 && gid <= 699999999) {
            return Type.PAGE;
        }
        if (gid < (1L << TYPE_SHIFT)) {
            return null;
        }
        long value = (gid >> TYPE_SHIFT);
        return Type.toType(value);
    }

    /**
     * 判断指定的gid是否合法，
     * 目前只判断最小值。
     * @param gid
     * @return
     */
    public static boolean isValidGid(long gid) {
        return gid >= (1L << TYPE_SHIFT);
    }
    
    //------------------------------------------

    private long id;

    protected G() {
    }

    public G(long id) {
        this.id = id;
    }

    public long getId() {
        return id;
    }

    public void setId(long gid) {
        this.id = gid;
    }

    public Type getType() {
        return G.extractType(id);
    }

    public long getX() {
        return G.extractX(id);
    }

    /**
     * 显示其类型以及源ID
     * 
     * @see Type#toString(long)
     */
    @Override
    public String toString() {
        return getType().toString(getX());
    }

}

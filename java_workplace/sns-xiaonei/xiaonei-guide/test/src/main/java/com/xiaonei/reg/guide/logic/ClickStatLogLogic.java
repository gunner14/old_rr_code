package com.xiaonei.reg.guide.logic;

import com.xiaonei.reg.guide.dao.ClickStatLogDAO;

final public class ClickStatLogLogic {
    /**
     * 单例
     */
    private ClickStatLogLogic() {
        super();
    }

    private static ClickStatLogLogic _instance = new ClickStatLogLogic();

    public static ClickStatLogLogic getInstance() {
        return _instance;
    }

    /**
     * <ul>
     * <li>插入点击日志:::采用异步批量插入优化</li>
     * </ul>
     *
     * @param userid
     * @param userstage
     * @param logincount
     * @param group
     * @param linkid
     * @param ext
     */
    public void insertCountClick(int bid, int userid, int type1, int type2,
                                 int type3, int type4, String str) {
        ClickStatLogDAO.getInstance().insertCountClick(bid, userid, type1, type2,
				type3, type4, str);
	}
}

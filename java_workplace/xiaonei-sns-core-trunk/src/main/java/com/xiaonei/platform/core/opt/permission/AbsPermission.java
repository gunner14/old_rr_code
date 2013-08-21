package com.xiaonei.platform.core.opt.permission;

import java.util.List;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.xiaonei.platform.core.opt.permission.define.Action;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;

/**
 * 权限检查抽象类，每一个继承类完成一个module的权限检查。
 * 
 * @author yunlong.bai@opi-corp.com
 * 
 */
public abstract class AbsPermission implements Permission {

    private Logger logger = LogManager.getLogger(getClass());

    private StringBuilder logSb = null;

    private List<Strategy> selectStrategy = null;

    private List<Strategy> insertStrategy = null;

    private List<Strategy> updateStrategy = null;

    private List<Strategy> deleteStrategy = null;

    public void addStrages(String action, List<Strategy> strategys) {

        if (action.equals(Action.ACTION_DELETE)) this.deleteStrategy = strategys;
        if (action.equals(Action.ACTION_INSERT)) this.insertStrategy = strategys;
        if (action.equals(Action.ACTION_SELECT)) this.selectStrategy = strategys;
        if (action.equals(Action.ACTION_UPDATE)) this.updateStrategy = strategys;

    }

    public List<Strategy> getSelect() {
        return this.selectStrategy;
    }

    public List<Strategy> getInsert() {
        return this.insertStrategy;
    }

    public List<Strategy> getUpdate() {
        return this.updateStrategy;
    }

    public List<Strategy> getDelete() {
        return this.deleteStrategy;
    }

    /**
     * 权限检查方法
     * 
     * @param action 行为
     * @param guester 访问者status
     * @param owner 被访问者status
     * @return 检查结果对象
     */
    public Ret hasPermission(String action, Source guester, Source owner) {
        if (Action.ACTION_DELETE.equals(action)) return this.hasDelete(guester, owner);
        if (Action.ACTION_INSERT.equals(action)) return this.hasInsert(guester, owner);
        if (Action.ACTION_SELECT.equals(action)) return this.hasSelect(guester, owner);
        if (Action.ACTION_UPDATE.equals(action)) return this.hasUpdate(guester, owner);
        return new Ret(false, false, "");
    }

    public Ret hasSelect(Source guester, Source source) {
        return this.hasRight(this.selectStrategy, guester, source);
    }

    public Ret hasUpdate(Source guester, Source source) {
        return this.hasRight(this.updateStrategy, guester, source);
    }

    public Ret hasInsert(Source guester, Source source) {
        return this.hasRight(this.insertStrategy, guester, source);
    }

    public Ret hasDelete(Source guester, Source source) {
        return this.hasRight(this.deleteStrategy, guester, source);
    }

	private Ret hasRight(List<Strategy> strategys, Source guester, Source source) {

		if (guester == null || source == null)
			return new Ret(false, false, "");
		if (strategys == null)
			return new Ret(false, true, "");
		if (logger.isDebugEnabled())
			logSb = new StringBuilder();

		String rtClass = "";
		try {
			for (int i = 0; i < strategys.size(); i++) {
				if (logger.isDebugEnabled())
					strategys.get(i).setLogAppender(logSb);
				Ret rt = strategys.get(i).doStrategy(guester, source);
				// System.out.println("className:"+strategys.get(i).getClassName()+":"+rt)
				// ;
				if (rt.isComeon())
					continue;
				else {
					if (logger.isDebugEnabled()) {
						if (rt.hasRight()) {
							rtClass = "pass";
						} else {
							rtClass = strategys.get(i).getClass()
									.getSimpleName();
						}
					}
					return rt;
				}
			}
			Ret rt = new Ret(false, false,
					"com.xiaonei.platform.core.opt.permission.strategy.impl.Default");
			// System.out.println(rt) ;
			return rt;
		} finally {
			if (logger.isDebugEnabled()) {
				logger.debug(logSb.toString());
				logger.debug("rt:" + rtClass);
			}
		}
	}

}

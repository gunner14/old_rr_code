package com.xiaonei.platform.core.opt.base.chain.impl.struts;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.chain.impl.WebContextBase;


/**
 * 基于Action的Context实现。
 * 
 * @author Li Weibo
 * @since 2008-12-28 下午10:17:56
 */
public class ActionContext extends WebContextBase {
	
	private static final long serialVersionUID = -5542850724811334369L;

	private ActionMapping actionMapping;
	
	private ActionForm actionForm;
	
	public ActionMapping getActionMapping() {
		return actionMapping;
	}
	public void setActionMapping(ActionMapping actionMapping) {
		this.actionMapping = actionMapping;
	}
	public ActionForm getActionForm() {
		return actionForm;
	}
	public void setActionForm(ActionForm actionForm) {
		this.actionForm = actionForm;
	}
}

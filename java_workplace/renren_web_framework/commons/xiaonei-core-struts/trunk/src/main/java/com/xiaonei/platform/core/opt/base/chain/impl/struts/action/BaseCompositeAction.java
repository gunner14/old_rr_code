package com.xiaonei.platform.core.opt.base.chain.impl.struts.action;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.Mediator;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.ActionContext;
import com.xiaonei.xce.log.TimeCost;

/**
 * 使用组合模式的Action的基类，这个类直接初始化和调用Mediator。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午08:52:15
 */
public abstract class BaseCompositeAction extends Action {

	protected Mediator mediator = new Mediator();
	
	protected final static Log chainTimeCostLogger = BizFilterUtil.getTimeCostLog();
	
	public BaseCompositeAction(InputStream is) {
		super();
		try {
			mediator.loadConfig(is);	//load mediator配置
			
			//删除掉要屏蔽的filter
			List<String> disabledFilters = getDisabledFilterNames();
			if (disabledFilters != null) {
				for (String filterName : disabledFilters) {
					mediator.deleteFilter(filterName);
				}
			}
		} catch (Exception e) {
			if (e instanceof RuntimeException) {
				throw (RuntimeException)e;
			} else {
				throw new RuntimeException("Exception is thrown while initializing.", e);
			}
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	/**
	 * 子类重写此方法以指定配置文件
	 * @return 配置文件的输入流
	 */
	//protected abstract InputStream getConfigFileAsStream();
	
	/**
	 * 子类可以重写此方法以屏蔽部分Filter
	 * @return 要屏蔽的Filter的类名集合
	 */
	protected List<String> getDisabledFilterNames () {
		return null;
	}
	
	@Override
	public final ActionForward execute(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		//construct a context to execute the command
		ActionContext context = new ActionContext();
		context.setActionMapping(mapping);
		context.setActionForm(form);
		context.setRequest(request);
		context.setResponse(response);
		TimeCost timecost = TimeCost.begin(chainTimeCostLogger, "BizFilterChain");
		try {
			context.put("timecost", timecost);
			boolean ret = mediator.execute(context);
			if (ret) {	//返回true说明mediator请求中止后续操作
				return null;
			}
		} finally {
			timecost.endFinally();
		}
		
		return executeBt(mapping, form, request, response);
	}

	public void addMsg(HttpServletRequest request, HttpServletResponse response, String msg) {
		BizFilterUtil.addMsg(request, response, msg);
	}

	public void addErr(HttpServletRequest request, HttpServletResponse response, String msg) {
		BizFilterUtil.addErr(request, response, msg);
	}
	
	public void addMsg(HttpServletRequest request, String msg) {
		BizFilterUtil.addMsg(request, msg);
	}

	public void addErr(HttpServletRequest request, String msg) {
		BizFilterUtil.addErr(request, msg);
	}
	
	public abstract ActionForward executeBt (ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response);
}

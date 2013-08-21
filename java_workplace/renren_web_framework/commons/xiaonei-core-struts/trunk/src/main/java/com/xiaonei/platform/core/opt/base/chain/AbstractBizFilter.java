package com.xiaonei.platform.core.opt.base.chain;

import org.apache.commons.chain.Context;
import org.apache.log4j.Logger;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.xce.log.TimeCost;

/**
 * BizFilter的一个抽象实现，在BizFilter的基础上加入了beforeExecute和afterExecute这两个切片方法，以方便进行统一的统计操作。
 * 具体的BizFilter实现类可以继承自此抽象类。
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午10:09:29
 */
public abstract class AbstractBizFilter implements BizFilter {

	protected static Logger logger = BizFilterUtil.getLogger();
	
	/**
	 * 此filter的名字，也应该是此filter的唯一标识
	 */
	private String name;
	
	@Override
	public void init(BizFilterConfig filterConfig) {
		name = filterConfig.getFilterName();
	}
	
	/**
	 * 在执行executeMe方法之前执行此方法，子类可以重写此方法来完成统计工作。
	 * @param context
	 * @throws BizFilterException
	 */
	public void beforeExecute(WebContext context) throws BizFilterException {
		//do nothing
		if (logger.isDebugEnabled()) {
			logger.debug(this.getClass().getName() + " ====> beforeExecute");
		}
		TimeCost timecost = (TimeCost) context.get("timecost");
		if (timecost != null) {
			timecost.endStep(this.getClass().getName() + " ==> beforeExecute");
		}
	}
	
	/**
	 * 先执行beforeExecute方法，再执行模板方法executeMe，最后执行afterExecute方法。
	 * 
	 * 这三个方法的执行过程中，有任意一个抛出了BizFilterException，后面的流程就会被打断。
	 * 
	 * @param context
	 * @throws BizFilterException
	 */
	@Override
	public final boolean execute(Context context) throws BizFilterException {
		if (!(context instanceof WebContext)) {
			throw new IllegalArgumentException("Parameter type should be " + WebContext.class.getName());
		}
		WebContext webcontext = (WebContext) context;
		beforeExecute(webcontext);
		if (logger.isDebugEnabled()) {
			logger.debug(this.getClass().getName() + " ====> executeMe");
		}
		boolean ret = executeMe(webcontext);
		afterExecute(webcontext);
		return ret;
	}

	/**
	 * 子类实现此方法
	 * 
	 * @param context
	 * @return
	 * @throws BizFilterException
	 */
	public abstract boolean executeMe(WebContext context) throws BizFilterException;
	
	/**
	 * 在执行executeMe之后执行此方法，子类可以重写此方法来完成统计工作。
	 * @param context
	 * @throws BizFilterException
	 */
	public void afterExecute(WebContext context) throws BizFilterException {
		//do nothing
		if (logger.isDebugEnabled()) {
			logger.debug(this.getClass().getName() + " ====> afterExecute");
		}
		TimeCost timecost = (TimeCost) context.get("timecost");
		if (timecost != null) {
			timecost.endStep(this.getClass().getName() + " ==> afterExecute");
		}
	}
	
	@Override
	public boolean postprocess(Context context, Exception exception) {
		return false;
	}

	public String getName() {
		return name;
	}

}

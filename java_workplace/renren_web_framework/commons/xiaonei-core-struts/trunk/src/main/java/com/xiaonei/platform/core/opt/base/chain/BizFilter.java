package com.xiaonei.platform.core.opt.base.chain;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.Filter;

/**
 * 业务逻辑过滤器的接口。每一个独立的业务逻辑都可以构造一个类实现此接口。
 * 
 * @author Li Weibo
 * @since 2008-12-28 上午12:12:35
 */
public interface BizFilter extends Filter {
	
	public void init(BizFilterConfig filterConfig);
	
	/**
     * <p>Execute a unit of processing work to be performed.  This
     * {@link Command} may either complete the required processing
     * and return <code>true</code>, or delegate remaining processing
     * to the next {@link Command} in a {@link Chain} containing this
     * {@link Command} by returning <code>false</code>
     *
     * @param context The {@link Context} to be processed by this
     *  {@link Command}
     *
     * @exception BizFilterException general purpose exception return
     *  to indicate abnormal termination
     * @exception IllegalArgumentException if <code>context</code>
     *  is <code>null</code>
     *
     * @return <code>true</code> if the processing of this {@link Context}
     *  has been completed, or <code>false</code> if the processing
     *  of this {@link Context} should be delegated to a subsequent
     *  {@link Command} in an enclosing {@link Chain}
     */
	public boolean execute(Context context) throws BizFilterException;
}

/**
 * 
 */
package com.xiaonei.platform.component.portal;

import java.util.Map;

/**
 * <ul>
 * <li>
 * portlet执行类
 * 
 * @author skllb </li> <li>2009-3-19 下午10:46:02</li> <li></li>
 *         </ul>
 */
public interface IPortletExecutor
{
	/**
	 * <ul>
	 * <li>portlet计算部分,需要将多个执行环境构造成一个map返回</li>
	 * </ul>
	 * 
	 * @param ctx
	 *            执行环境
	 * @return 结果集，使用map是未了保证一个portlet中多个执行的原子性
	 */
	public Map<String, Object> execute(PortalContext ctx);

	/**
	 * <ul>
	 * <li>健康检查</li>
	 * </ul>
	 * 
	 * @return
	 */
	public boolean ping();
}

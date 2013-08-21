/**
 * 
 */
package com.xiaonei.platform.component.portal;

/**
 * <ul>
 * <li>
 * 执行环境提供者::::通过系统统一调用执行者，用来实现延迟加载
 * 
 * @author skllb </li> <li>2009-3-19 下午10:49:17</li> <li></li>
 *         </ul>
 */
public interface IContextprovider
{
	/**
	 * <ul>
	 * <li>获取该环境变量的key</li>
	 * </ul>
	 * 
	 * @return
	 */
	public String getKey();

	/**
	 * <ul>
	 * <li>获取对应当前key的执行环境变量</li>
	 * </ul>
	 * 
	 * @param ctx
	 * @return
	 */
	public Object synchrData(PortalContext ctx);
}
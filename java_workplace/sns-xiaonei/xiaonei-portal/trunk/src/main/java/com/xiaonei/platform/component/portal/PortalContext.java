/**
 * 
 */
package com.xiaonei.platform.component.portal;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;

/**
 * <ul>
 * <li>
 * portlet执行的环境
 * 
 * @author skllb </li> <li>2009-3-19 下午10:48:47</li> <li></li>
 *         </ul>
 */
public class PortalContext
{
	// request中的所有参数map
	private Map<String, String[]> requestParams = null;

	// 执行环境存储地址
	private Map<String, Object> ctxMap = new HashMap<String, Object>();

	// 结果存储地址
	private Map<String, Object> resultMap = new HashMap<String, Object>();

	public Map<String, String[]> getRequestParams()
	{
		return requestParams;
	}

	/**
	 * <ul>
	 * <li>获取一个request参数</li>
	 * </ul>
	 * 
	 * @param name
	 * @return
	 */
	public String getRequestParam(String name)
	{
		String[] ps = requestParams.get(name);
		if (ps != null && ps.length > 0)
		{
			return ps[0];
		}
		return null;
	}

	public void setRequestParams(Map<String, String[]> requestParams)
	{
		this.requestParams = requestParams;
	}

	/**
	 * 构造函数
	 * 
	 * @param requestParamsMap
	 *            request中的所有参数map,通过httpServletRequest.getParameterMap获得
	 */
	public PortalContext(Map<String, String[]> requestParamsMap)
	{
		this.requestParams = requestParamsMap;
	}

	/**
	 * 构造函数
	 * 
	 * @param requestParamsMap
	 *            request中的所有参数map,通过httpServletRequest.getParameterMap获得
	 * @param otherParams
	 *            其他的环境参数,将附属进入ctxTable
	 */
	public PortalContext(Map<String, String[]> requestParamsMap,
			Map<String, Object> initParams)
	{
		this.requestParams = requestParamsMap;
		ctxMap.putAll(initParams);
	}

	/**
	 * <ul>
	 * <li>获取执行环境</li>
	 * </ul>
	 * 
	 * @param key
	 * @return
	 */
	public Object get(IContextprovider ctxProvider)
	{
		Object o = ctxMap.get(ctxProvider.getKey());
		if (o == null)
		{
			// synchronized (this)
			// {// 恶心的双锁定机制
			// o = ctxMap.get(ctxProvider.getKey());
			// if (o == null)
			// {
			o = ctxProvider.synchrData(this);
			if (o != null)
			{
				ctxMap.put(ctxProvider.getKey(), o);
			}
			return o;
			// }
			// return o;
			// }
		} else
		{
			return o;
		}
	}

	/**
	 * <ul>
	 * <li>获取执行环境</li>
	 * </ul>
	 * 
	 * @param key
	 * @return
	 */
	public Object get(String key)
	{
		return ctxMap.get(key);
	}

	/**
	 * <ul>
	 * <li>存储执行结果</li>
	 * </ul>
	 * 
	 * @param name
	 * @param value
	 */
	void addResults(Map<String, Object> re)
	// synchronized void addResults(Map<String, Object> re)
	{
		this.resultMap.putAll(re);
	}

	/**
	 * <ul>
	 * <li>获取执行结果</li>
	 * </ul>
	 * 
	 * @param name
	 * @return
	 */
	public Object getResult(String name)
	{
		return this.resultMap.get(name);
	}

	/**
	 * <ul>
	 * <li>获取所有执行结果</li>
	 * </ul>
	 * 
	 * @param name
	 * @return
	 */
	public Map<String, Object> getResults()
	{
		return this.resultMap;
	}

	/**
	 * <ul>
	 * <li>将所有计算结果放入到requestScope中</li>
	 * </ul>
	 * 
	 * @param request
	 */
	public void initRequest(HttpServletRequest request)
	{
		for (Entry<String, Object> ety : resultMap.entrySet())
		{
			request.setAttribute(ety.getKey(), ety.getValue());
		}
	}
}

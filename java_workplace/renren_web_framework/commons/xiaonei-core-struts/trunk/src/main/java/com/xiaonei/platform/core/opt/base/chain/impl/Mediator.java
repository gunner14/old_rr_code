package com.xiaonei.platform.core.opt.base.chain.impl;

import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import com.xiaonei.platform.core.opt.base.chain.BizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterConfig;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.BizFilterExceptionHandler;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;

/**
 * 
 * 中介者(Mediator)模式的定义是用一个中介对象来封装一系列的对象交互，中介者使各对象不需要显式地相互引用。
 * 
 * 这个中介者控制BizChainBase, BizFilter和BizFilterExceptionHandler的生命周期，并将它们之间的交互封装起来，只对上层调用者提供简单的接口。
 * 
 * @author Li Weibo
 * @since 2008-12-28 上午11:17:37
 */
public class Mediator implements BizFilter {

	private BizChainBase chain = new BizChainBase();
	
	private HashMap<Integer, BizFilterExceptionHandler> exceptionHandlerMap = new HashMap<Integer, BizFilterExceptionHandler>();
	
	private static final Logger chainLogger = BizFilterUtil.getLogger();
	
	/**
	 * 锁定标志，frozen为true的时候不能再进行对chain的操作
	 */
	boolean frozen = false;
	
	/**
	 * 添加一个filter
	 * @param filter 待添加的filter
	 */
	public void addFilter(BizFilter filter) {
		checkFrozen();
		if (chainLogger.isDebugEnabled()) {
			chainLogger.debug("Add to chain : " + filter.getClass().getName());
		}
		chain.addCommand(filter);
	}
	
	/**
	 * 向指定位置插入一个filter
	 * 
	 * @param index 要插入的位置
	 * @param filter 待插入的filter
	 */
	public void addFilter(int index, BizFilter filter) {
		checkFrozen();
		if (chainLogger.isDebugEnabled()) {
			chainLogger.debug("Add to chain at index " + index + " : " + filter.getClass().getName());
		}
		chain.addFilter(index, filter);
	}
	
	/**
	 * 按name或className删除一个filter
	 * @param filterName 要删除的filter的name
	 */
	public void deleteFilter(String filterName) {
		checkFrozen();
		if (chainLogger.isDebugEnabled()) {
			chainLogger.debug("Delete filter : " + filterName);
		}
		chain.deleteFilter(filterName);
	}
	
	/**
	 * 按名字获取一个chain中的一个Filter，先按filterName查找，再按className查找。
	 * 找不到则返回null
	 * @param filterName 
	 * @return
	 */
	public BizFilter getFilter(String filterName) {
		checkFrozen();
		if (chainLogger.isDebugEnabled()) {
			chainLogger.debug("Get filter : " + filterName);
		}
		return chain.getFilter(filterName);
	}
	
	/**
	 * 检查chain是否已经被冻结
	 */
	private void checkFrozen() {
		if (frozen) {
			throw new IllegalStateException("You can not do this operation after first time the execute method is invoked.");
		}
	}
	
	@SuppressWarnings("unchecked")
	public void loadConfig(InputStream is) {
		if (chainLogger.isDebugEnabled()) {
			chainLogger.debug("Loading mediator configuration ...");
		}
		SAXReader reader = new SAXReader();
		try {
			Document document = reader.read(is);
			Element root = document.getRootElement();
			//load filter chain
			List<Element> filterNodes = root.elements("filter");
			for (Element node : filterNodes) {
				String className = node.element("filter-class").getText();
				BizFilter filterObj = (BizFilter) createObject(className);
				String filterName = node.element("filter-name").getText();
				if (filterName == null) {
					throw new RuntimeException("'filter-name' is required.");
				}
				
				//load config
				BizFilterConfigBase config = new BizFilterConfigBase();
				List<Element> paramNodes = node.elements("init-param");
				config.setFilterName(filterName);
				for (Element paramNode : paramNodes) {
					String key = paramNode.elementText("param-name");
					String value = paramNode.elementText("param-value");
					config.setParam(key, value);
				}
				//init filter
				filterObj.init(config);
				addFilter(filterObj);
			}
			
			//load exception handlers
			List<Element> handlerNodes = root.elements("exception-handler");
			for (Element handlerNode : handlerNodes) {
				String s_code = handlerNode.elementText("code");
				if (s_code == null) {
					throw new RuntimeException("'code' is required.");
				}
				int code = Integer.parseInt(s_code);
				String className = handlerNode.elementText("handler-class");
				BizFilterExceptionHandler handlerObj = (BizFilterExceptionHandler) createObject(className);
				setExceptionHandler(code, handlerObj);
			}
		} catch (Exception e) {
			throw new RuntimeException("Exception is thrown while loading chain configuration.", e);
		}
	}
	
	@SuppressWarnings("unchecked")
	private Object createObject(String className) throws Exception {
		Class klass = Class.forName(className);
		Constructor constructor = klass.getConstructor();
		return constructor.newInstance();
	}
	
	public void setExceptionHandler(int code, BizFilterExceptionHandler handler) {
		exceptionHandlerMap.put(code, handler);
	}
	
	public boolean execute(Context context) {
		frozen = true;	//frozen the chain
		if (chainLogger.isDebugEnabled()) {
			chainLogger.debug("Executing the chain ...");
		}
		boolean ret = false;
		try {
			ret = chain.execute(context);
		} catch (BizFilterException e) {
			if (chainLogger.isInfoEnabled()) {
				chainLogger.info("Encounter error : " + e.getCode());
			}
			//fetch proper handler for this exception
			BizFilterExceptionHandler handler = exceptionHandlerMap.get(e.getCode());
			if (handler == null) {
				String msg = "No proper handler for exception:" + e.getCode();
				chainLogger.error(msg);
				throw new RuntimeException(msg, e);
			}
			//System.out.println(e.getCode() + " is handled by " + handler.getClass().getName());
			//handle it
			if (chainLogger.isInfoEnabled()) {
				chainLogger.info("Handling error " + e.getCode() + " using handler : " + handler.getClass().getName());
			}
			handler.handle(e, context);
			return true;	//发生了异常，return true通知调用者中止后续操作
		}
		return ret;
	}

	public boolean postprocess(Context context, Exception exception) {
		return false;
	}

	@Override
	public void init(BizFilterConfig filterConfig) {
		// TODO Auto-generated method stub
	}
	
	public static void main(String[] args) {
		Mediator m = new Mediator();
		m.loadConfig(BasePageAction.class.getResourceAsStream("base-page-action-config.xml"));
		System.out.println(m);
	}
}

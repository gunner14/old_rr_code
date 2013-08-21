/*
 * Copyright 2009-2010 Oak Pacific Interactive. All rights reserved.
 */
package com.xiaonei.platform.core.opt.base.wrapper;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;

/**
 * 应用公司自己定义的request、response包装器，以实现自己特有的request和response策略
 * <p>
 * 
 * 请将此 {@link Filter} 配置到web.xml中所有filter的最前面。参考配置如下：
 * 
 * <pre>
 * &lt;filter&gt;
 *    &lt;filter-name&gt;OpiWrapperFilter&lt;/filter-name&gt;
 *    &lt;filter-class&gt;com.xiaonei.platform.core.opt.base.wrapper.OpiWrapperFilter&lt;/filter-class&gt;
 * &lt;/filter&gt;
 * &lt;filter-mapping&gt;
 *    &lt;filter-name&gt;OpiWrapperFilter&lt;/filter-name&gt;
 *    &lt;url-pattern&gt;/*&lt;/url-pattern&gt;
 * &lt;/filter-mapping&gt;
 * 
 * <pre>
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public class OpiWrapperFilter implements Filter {

    @Override
    public void init(FilterConfig config) throws ServletException {

    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        ServletRequest requestWrapper = new OpiHttpServletRequestWrapper(
                (HttpServletRequest) request);
        chain.doFilter(requestWrapper, response);
    }

    @Override
    public void destroy() {

    }

}

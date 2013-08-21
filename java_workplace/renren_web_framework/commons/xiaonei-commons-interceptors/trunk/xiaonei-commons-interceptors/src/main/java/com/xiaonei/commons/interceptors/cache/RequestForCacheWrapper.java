/**
 * 
 */
package com.xiaonei.commons.interceptors.cache;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.Principal;
import java.util.Enumeration;
import java.util.Locale;
import java.util.Map;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletInputStream;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 对Reuqest进行封装，由于Cache是只存Model对象里的数据，那么如果有调用setAttribute，就WARN一下
 * 
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public class RequestForCacheWrapper implements HttpServletRequest {

    private final Log logger = LogFactory.getLog(RequestForCacheWrapper.class.getName());

    private HttpServletRequest request = null;

    public RequestForCacheWrapper(final HttpServletRequest request) {
        this.request = request;
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.ServletRequest#getAttribute(java.lang.String)
     */
    @Override
    public Object getAttribute(final String arg0) {
        return this.request.getAttribute(arg0);
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getAttributeNames()
     */
    @Override
    @SuppressWarnings("rawtypes")
    public Enumeration getAttributeNames() {
        return this.request.getAttributeNames();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getAuthType()
     */
    @Override
    public String getAuthType() {
        return this.request.getAuthType();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getCharacterEncoding()
     */
    @Override
    public String getCharacterEncoding() {
        return this.request.getCharacterEncoding();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getContentLength()
     */
    @Override
    public int getContentLength() {
        return this.request.getContentLength();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getContentType()
     */
    @Override
    public String getContentType() {
        return this.request.getContentType();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getContextPath()
     */
    @Override
    public String getContextPath() {
        return this.request.getContextPath();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getCookies()
     */
    @Override
    public Cookie[] getCookies() {
        return this.request.getCookies();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.http.HttpServletRequest#getDateHeader(java.lang.String)
     */
    @Override
    public long getDateHeader(final String arg0) {
        return this.request.getDateHeader(arg0);
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.http.HttpServletRequest#getHeader(java.lang.String)
     */
    @Override
    public String getHeader(final String arg0) {
        return this.request.getHeader(arg0);
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getHeaderNames()
     */
    @Override
    @SuppressWarnings("rawtypes")
    public Enumeration getHeaderNames() {
        return this.request.getHeaderNames();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.http.HttpServletRequest#getHeaders(java.lang.String)
     */
    @Override
    @SuppressWarnings("rawtypes")
    public Enumeration getHeaders(final String arg0) {
        return this.request.getHeaders(arg0);
    }

    /**
     * @return
     * @throws IOException
     * @see javax.servlet.ServletRequest#getInputStream()
     */
    @Override
    public ServletInputStream getInputStream() throws IOException {
        return this.request.getInputStream();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.http.HttpServletRequest#getIntHeader(java.lang.String)
     */
    @Override
    public int getIntHeader(final String arg0) {
        return this.request.getIntHeader(arg0);
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getLocalAddr()
     */
    @Override
    public String getLocalAddr() {
        return this.request.getLocalAddr();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getLocale()
     */
    @Override
    public Locale getLocale() {
        return this.request.getLocale();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getLocales()
     */
    @Override
    @SuppressWarnings("rawtypes")
    public Enumeration getLocales() {
        return this.request.getLocales();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getLocalName()
     */
    @Override
    public String getLocalName() {
        return this.request.getLocalName();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getLocalPort()
     */
    @Override
    public int getLocalPort() {
        return this.request.getLocalPort();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getMethod()
     */
    @Override
    public String getMethod() {
        return this.request.getMethod();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.ServletRequest#getParameter(java.lang.String)
     */
    @Override
    public String getParameter(final String arg0) {
        return this.request.getParameter(arg0);
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getParameterMap()
     */
    @Override
    @SuppressWarnings("rawtypes")
    public Map getParameterMap() {
        return this.request.getParameterMap();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getParameterNames()
     */
    @Override
    @SuppressWarnings("rawtypes")
    public Enumeration getParameterNames() {
        return this.request.getParameterNames();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.ServletRequest#getParameterValues(java.lang.String)
     */
    @Override
    public String[] getParameterValues(final String arg0) {
        return this.request.getParameterValues(arg0);
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getPathInfo()
     */
    @Override
    public String getPathInfo() {
        return this.request.getPathInfo();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getPathTranslated()
     */
    @Override
    public String getPathTranslated() {
        return this.request.getPathTranslated();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getProtocol()
     */
    @Override
    public String getProtocol() {
        return this.request.getProtocol();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getQueryString()
     */
    @Override
    public String getQueryString() {
        return this.request.getQueryString();
    }

    /**
     * @return
     * @throws IOException
     * @see javax.servlet.ServletRequest#getReader()
     */
    @Override
    public BufferedReader getReader() throws IOException {
        return this.request.getReader();
    }

    /**
     * @param arg0
     * @return
     * @deprecated
     * @see javax.servlet.ServletRequest#getRealPath(java.lang.String)
     */
    @Deprecated
    @Override
    public String getRealPath(final String arg0) {
        return this.request.getRealPath(arg0);
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getRemoteAddr()
     */
    @Override
    public String getRemoteAddr() {
        return this.request.getRemoteAddr();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getRemoteHost()
     */
    @Override
    public String getRemoteHost() {
        return this.request.getRemoteHost();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getRemotePort()
     */
    @Override
    public int getRemotePort() {
        return this.request.getRemotePort();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getRemoteUser()
     */
    @Override
    public String getRemoteUser() {
        return this.request.getRemoteUser();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.ServletRequest#getRequestDispatcher(java.lang.String)
     */
    @Override
    public RequestDispatcher getRequestDispatcher(final String arg0) {
        return this.request.getRequestDispatcher(arg0);
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getRequestedSessionId()
     */
    @Override
    public String getRequestedSessionId() {
        return this.request.getRequestedSessionId();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getRequestURI()
     */
    @Override
    public String getRequestURI() {
        return this.request.getRequestURI();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getRequestURL()
     */
    @Override
    public StringBuffer getRequestURL() {
        return this.request.getRequestURL();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getScheme()
     */
    @Override
    public String getScheme() {
        return this.request.getScheme();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getServerName()
     */
    @Override
    public String getServerName() {
        return this.request.getServerName();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#getServerPort()
     */
    @Override
    public int getServerPort() {
        return this.request.getServerPort();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getServletPath()
     */
    @Override
    public String getServletPath() {
        return this.request.getServletPath();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getSession()
     */
    @Override
    public HttpSession getSession() {
        return this.request.getSession();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.http.HttpServletRequest#getSession(boolean)
     */
    @Override
    public HttpSession getSession(final boolean arg0) {
        return this.request.getSession(arg0);
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#getUserPrincipal()
     */
    @Override
    public Principal getUserPrincipal() {
        return this.request.getUserPrincipal();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#isRequestedSessionIdFromCookie()
     */
    @Override
    public boolean isRequestedSessionIdFromCookie() {
        return this.request.isRequestedSessionIdFromCookie();
    }

    /**
     * @return
     * @deprecated
     * @see javax.servlet.http.HttpServletRequest#isRequestedSessionIdFromUrl()
     */
    @Deprecated
    @Override
    public boolean isRequestedSessionIdFromUrl() {
        return this.request.isRequestedSessionIdFromUrl();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#isRequestedSessionIdFromURL()
     */
    @Override
    public boolean isRequestedSessionIdFromURL() {
        return this.request.isRequestedSessionIdFromURL();
    }

    /**
     * @return
     * @see javax.servlet.http.HttpServletRequest#isRequestedSessionIdValid()
     */
    @Override
    public boolean isRequestedSessionIdValid() {
        return this.request.isRequestedSessionIdValid();
    }

    /**
     * @return
     * @see javax.servlet.ServletRequest#isSecure()
     */
    @Override
    public boolean isSecure() {
        return this.request.isSecure();
    }

    /**
     * @param arg0
     * @return
     * @see javax.servlet.http.HttpServletRequest#isUserInRole(java.lang.String)
     */
    @Override
    public boolean isUserInRole(final String arg0) {
        return this.request.isUserInRole(arg0);
    }

    /**
     * @param arg0
     * @see javax.servlet.ServletRequest#removeAttribute(java.lang.String)
     */
    @Override
    public void removeAttribute(final String arg0) {
        this.request.removeAttribute(arg0);
    }

    /**
     * @param arg0
     * @param arg1
     * @see javax.servlet.ServletRequest#setAttribute(java.lang.String,
     *      java.lang.Object)
     */
    @Override
    public void setAttribute(final String arg0, final Object arg1) {
        this.logger.warn("Deprecated Call! with ResultCache, all Object in model" + //
                " will be cache,but Attribute don't! Object:" + arg1.toString());
        if (this.logger.isDebugEnabled()) {
            for (final StackTraceElement stackElement : Thread.currentThread().getStackTrace()) {
                this.logger.debug("\tat " + stackElement);
            }
        }
        this.request.setAttribute(arg0, arg1);
    }

    /**
     * @param arg0
     * @throws UnsupportedEncodingException
     * @see javax.servlet.ServletRequest#setCharacterEncoding(java.lang.String)
     */
    @Override
    public void setCharacterEncoding(final String arg0) throws UnsupportedEncodingException {
        this.request.setCharacterEncoding(arg0);
    }
}

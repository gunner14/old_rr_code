package com.xiaonei.login.logic;

import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLDecoder;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;

@Service
public class DomainOprImpl implements DomainOpr {

	@Override
	public boolean currentDomainIsNotPassportDomain(
			final HttpServletRequest request) {
		return !StringUtils.endsWith(request.getServerName(),
				Globals.PASSPORT_DOMAIN);
	}

	@Override
	public String getDomainAsParameterFromCurUrl(HttpServletRequest request) {
		if (StringUtils.isEmpty(request.getParameter("domain"))) {
			String domain = StringUtils.endsWith(request.getServerName(),
					Globals.KAIXIN_DOMAIN) ? Globals.KAIXIN_DOMAIN
					: Globals.RENREN_DOMAIN;
			return "domain=" + domain;
		}
		return "";
	}

	@Override
	public String getDomainParam(String domain, HttpServletRequest request) {
		if (StringUtils.isEmpty(domain)) {
			domain = Globals.RENREN_DOMAIN;
		} else if (!isRenKaiDomain(domain)) {
			domain = Globals.RENREN_DOMAIN;
		}
		request.setAttribute(LoginParamDef.PARAM_DOMAIN, domain);
		return domain;
	}

	@Override
	public String getPassportDomainAddrFromCurrentAddr(final Invocation inv,
			final String origURL) {
		final User host = BaseThreadUtil.currentHost(inv.getRequest());
		if (null != host) {
			try {
				new URL(origURL);
				return origURL;
			} catch (MalformedURLException e) {
				try {
					return URLDecoder.decode(origURL, "UTF-8");
				} catch (UnsupportedEncodingException e1) {
					e1.printStackTrace();
				} catch (NullPointerException e1) {
					e1.printStackTrace();
				}
			}
		}

		HttpServletRequest request = inv.getRequest();
		String dm = getDomainAsParameterFromCurUrl(request);
		String queryString = request.getQueryString() == null ? "" : request
				.getQueryString();
		if (StringUtils.isNotEmpty(dm) && StringUtils.isNotEmpty(queryString)) {
			queryString = dm + "&" + queryString;
		} else {
			queryString = dm + queryString;
		}
		String rtUrl = "http://" + Globals.PASSPORT_DOMAIN + "/PLogin.do?"
				+ queryString;
		return rtUrl;
	}

	@Override
	public boolean isRenKaiDomain(String domain) {
		if (StringUtils.equals(Globals.RENREN_DOMAIN, domain)) {
			return true;
		}
		if (StringUtils.equals(Globals.KAIXIN_DOMAIN, domain)) {
			return true;
		}
		return false;
	}

	@Override
	public void setDomainEnv(HttpServletRequest request) {
		String domain = (String) request
				.getAttribute(LoginParamDef.PARAM_DOMAIN);
		setDomainEnv(request, domain);
	}

	@Override
	public void setDomainEnv(HttpServletRequest request, String domain) {
		if (null == request) {
			return;
		}
		if (StringUtils.isEmpty(domain)) {
			domain = getDomainParam(domain, request);
		}
		OpiConstants.setCurrentDomain(domain);
		request.setAttribute("domainIdentified", OpiConstants
				.getCurrentDomainIdentifiedProperties());
	}
}

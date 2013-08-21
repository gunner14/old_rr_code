package com.xiaonei.platform.taglib.name;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

import org.apache.taglibs.standard.lang.support.ExpressionEvaluatorManager;

import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * 网络名Tag，根据网络 id 返回网络名
 */

public class NetworkNameTag extends TagSupport{

	private Object networkId;

	public void setNetworkId(Object networkId) throws JspException {
		this.networkId = ExpressionEvaluatorManager.evaluate("networkId",networkId.toString(),Object.class,pageContext);
	}

	@Override
	public int doStartTag() throws JspException {
		if(networkId!=null){
			try {
				Integer netId = Integer.parseInt(networkId.toString());
				String networkName = NetworkManager.getInstance().getNetById(netId).getName() ;
				TagUtils.out(pageContext,networkName);
			} catch (NumberFormatException e) {
			}
		}
		return super.doStartTag();
	}

}

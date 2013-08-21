package com.xiaonei.platform.taglib.menu;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

import com.xiaonei.platform.taglib.helper.TempletData;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class FooterTag extends TagSupport {
	/**是否显示icp 值有show */
	private String icp ;
	
	/**是否显示网络警察 值有show */
	private String netpolice ;
	
	
	public String getIcp() {
		return icp;
	}


	public void setIcp(String icp) {
		this.icp = icp;
	}


	public String getNetpolice() {
		return netpolice;
	}


	public void setNetpolice(String netpolice) {
		this.netpolice = netpolice;
	}


	@Override
	public int doStartTag() throws JspException {
//		User host = (User) this.pageContext.getAttribute("host",
//				PageContext.REQUEST_SCOPE);//
//		String google = GoogleAnalyticsHelper.help(pageContext, host);
		String t1 = TempletData.getInstance().getMap(TempletData.Footer_LinkBar);//+ google;
		
		String t2 = TempletData.getInstance().getMap(TempletData.Footer_Icp);
		t2 = (t2==null?"":t2);
		
		String t3 = TempletData.getInstance().getMap(TempletData.Footer_Net_Police);
		t3 = (t3==null?"":t3);
		
		if(t1!=null){
			if(icp!=null && "show".equals(icp)){
				t1 = t1.replace("${Footer_Icp}", t2);
			}else{
				t1 = t1.replace("${Footer_Icp}", "");
			}

			if(netpolice!=null && "show".equals(icp)){
				t1 = t1.replace("${Footer_Net_Police}", t3);
			}else{
				t1 = t1.replace("${Footer_Net_Police}", "");
			}
		}
		
		TagUtils.out(pageContext, t1);
		return super.doStartTag();
	}
}
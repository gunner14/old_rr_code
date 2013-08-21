package com.xiaonei.platform.taglib.menu;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.PageContext;
import javax.servlet.jsp.tagext.TagSupport;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.platform.taglib.helper.TempletData;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class NavigationTag extends TagSupport {
    
    
    public NavigationTag() {
        throw new Error("Please contacts to zhiliang.wang@opi-corp.com; if this error happeneds");
    }
//
//	/**im 留言数*/
//	private String imnotice;
//	
//	@Override
//	public int doStartTag() throws JspException {
//		User host = (User) this.pageContext.getAttribute("host",PageContext.REQUEST_SCOPE);//
//		HttpServletRequest request = (HttpServletRequest) pageContext.getRequest();
//		String result = "";
//		if (host == null) {
//			// 未登录时
//			result = notLogin(request);
//		} else {
//			result = alreadyLogin( request, host);
//		}
//		
//		TagUtils.out(pageContext, result);
//		return super.doStartTag();
//	}
//	private String notLogin(HttpServletRequest request){
//		String t1 = TempletData.getInstance().getMap(TempletData.Navigation_Utility);
//		return t1;
//	}
//	private String alreadyLogin(HttpServletRequest request,User host){
//		String t1 = null;
//		t1 = TempletData.getInstance().getMap(TempletData.Navigation_NavLeft);
//		
//		String t2 = null;
//		t2 = TempletData.getInstance().getMap(TempletData.Navigation_NavRight);
//		
//		String t3 = null;
//		if(host.getStage()==10){
//			t3 = TempletData.getInstance().getMap(TempletData.Navigation_Replace_SuiBianKanKan);	
//		}
//		String t4 = null;
//		if(host.getStage()==20){
//			t4 = TempletData.getInstance().getMap(TempletData.Navigation_Replace_NewEvent);
//		}
//		String t5 = null;
//		if(host.getNewMessageCount()>0){
//			t5 = TempletData.getInstance().getMap(TempletData.Navigation_Replace_NewMessageCount);
//		}
//		
//		if(imnotice!=null && "show".equals(imnotice)){
//			try {
//				String t6 = null;
//				t6 = TempletData.getInstance().getMap(TempletData.Navigation_Replace_ImNotice);
//				int count = 0;
//				//count = mop.hi.oce.adapter.AdapterFactory.getCountCacheAdapter().getImLeavewordCount(host.getId());
//				count = UserCountMgr.getCount(host.getId(), UserCountMgr.IM_LEAVE_WORD);
//				if(count>0){
//					if(t6!=null && t6.trim().length()>0){
//						t6 = t6.replace("${Navigation_Replace_ImNotice_HostId}", String.valueOf(host.getId()));
//						t6 = t6.replace("${Navigation_Replace_ImNotice_Email}", host.getEmail());
//						t6 = t6.replace("${Navigation_Replace_ImNotice_Count}", String.valueOf(count));
//					}		
//				}
//				if(t2!=null){
//					if(t6!=null){
//						t6="";
//					}
//					t2 = t2.replace("${Navigation_Replace_ImNotice}", t6);
//				}
//			} catch (Exception e) {
//			}
//		}else{
//			t2 = t2.replace("${Navigation_Replace_ImNotice}", "");
//		}
//		
//		String result = "";
//		if(t1!=null){
//			result += t1;
//			if(t3!=null){
//				result = result.replace("${"+TempletData.Navigation_Replace_SuiBianKanKan+"}", t3);
//			}
//			if(t4!=null){
//				result = result.replace("${"+TempletData.Navigation_Replace_NewEvent+"}", t4);	
//			}
//			if(t5!=null){
//				t5 = t5.replace("${requestScope.host.newMessageCount}", String.valueOf(host.getNewMessageCount()));
//				result = result.replace("${"+TempletData.Navigation_Replace_NewMessageCount+"}", t5);
//			}
//		}
//		result += t2;
//		result = "<div id=\"navigation\">"+result+"</div>";
//		return result;
//	}
	
}
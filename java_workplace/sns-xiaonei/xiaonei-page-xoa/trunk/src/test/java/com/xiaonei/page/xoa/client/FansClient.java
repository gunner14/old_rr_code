package com.xiaonei.page.xoa.client;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.impl.listener.XoaBizErrorListener;
import com.xiaonei.page.xoa.api.RankService;
//import com.xiaonei.page.xoa.model.PageInfo;

// -Dxoa.hosts.page.xoa.renren.com=10.3.18.181:8188
public class FansClient {

//	public static void main(String[] args) throws Exception {
//		ServiceFactory fact = ServiceFactories.getFactory();
//		RankService rankService = fact.getService(RankService.class);
//		ServiceFuture<PageInfo[]> future = rankService.getFansRank();
//		XoaBizErrorListener bizErrorListener = new XoaBizErrorListener();
//		future.addListener(bizErrorListener);
//		future.submit();
//		if(future.await(200)) { // 超时
//			if (future.isSuccess()) { // 服务器正确返回
//				PageInfo[] pageInfos = future.getContent();
//				for (int i = 0; i < pageInfos.length; i++) {
//					PageInfo pi = pageInfos[i];
//					System.out.println(pi.getId() + " : " + pi.getName());
//				}
//			} else { // 服务器报错了
//				XoaBizErrorBean errorBean = bizErrorListener.getReturn();
//				if (errorBean != null) {
//					System.out.println("errorBean: " + errorBean.getMessage());
//				}
//			}
//		} else {
//			System.out.println("time out");
//		}
//
//	}

}

//package com.xiaonei.page.xoa.controllers.top;
//
//import java.util.ArrayList;
//import java.util.Calendar;
//import java.util.List;
//
//import net.paoding.rose.web.annotation.rest.Get;
//
//import org.springframework.beans.factory.annotation.Autowired;
//
//import com.xiaonei.page.dao.pop.TopFansDAO;
//import com.xiaonei.page.domain.Page;
//import com.xiaonei.page.domain.pop.TopFans;
//import com.xiaonei.page.service.PageService;
//import com.xiaonei.page.util.Keys;
//import com.xiaonei.page.util.MemcachedUtil;
//import com.xiaonei.page.xoa.model.PageInfo;
//
//public class FansController {
//
//	@Autowired
//	private TopFansDAO topFansDAO;
//	
//	@Autowired
//	private PageService pageService;
//	
//	private final static int TYPE_ID = 0;
//	
//	@Get
//	public PageInfo[] index() {
//		// 先取缓存
//		@SuppressWarnings("unchecked")
//		List<TopFans> topFansList = (List<TopFans>) MemcachedUtil.get(Keys.PAGE_FANS_TOPTEN, "");
//		if (topFansList == null) {
//			// 缓存失效，查db
//			System.out.println("缓存失效");
//			topFansList = getTopFansList(TYPE_ID);
//			if (topFansList.size() != 0) { // 有数据，则放入缓存
//				//MemcachedUtil.set(Keys.PAGE_FANS_TOPTEN, "", topFansList);
//			}
//		} else {
//			System.out.println("缓存命中");
//		}
//		
//		// 转换列表中对象的类型，同时过滤掉无用的page
//		List<PageInfo> pageInfoList = new ArrayList<PageInfo>();
//		if (topFansList != null) {
//			for (TopFans topFans : topFansList) {
//				Page page = pageService.get(topFans.getPageId());
//				if (page != null) {
//					PageInfo pi = new PageInfo();
//					pi.setId(page.getId());
//					pi.setName(page.getName());
//					pageInfoList.add(pi);
//				}
//			}
//		}
//		int size = pageInfoList.size();
//		System.out.println(size);
//		PageInfo[] pageInfos = (PageInfo[]) pageInfoList.toArray(new PageInfo[size]);
//		return pageInfos;
//	}
//	
//	private List<TopFans> getTopFansList(int typeId) {
//		
//		// 先从今天的表里取数据
//		Calendar cal = Calendar.getInstance();
//		int year = cal.get(Calendar.YEAR);
//		int month = cal.get(Calendar.MONTH) + 1;
//		int day = cal.get(Calendar.DAY_OF_MONTH);
//		List<TopFans> topFansList = topFansDAO.getCheckedList(typeId, year, month, day);
//		if (topFansList == null || topFansList.size() == 0) {
//			// 今天的表里没有数据，则取昨天的数据
//			cal.add(Calendar.DAY_OF_MONTH, -1);
//			year = cal.get(Calendar.YEAR);
//			month = cal.get(Calendar.MONTH) + 1;
//			day = cal.get(Calendar.DAY_OF_MONTH);
//			topFansList = topFansDAO.getCheckedList(typeId, year, month, day);
//		}
//		return topFansList; 
//		
//	}
//	
//}

/**
 * 
 */
package com.xiaonei.reg.register.controllers.ajax;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.photo.xoa.api.service.AlbumListService;
import com.renren.photo.xoa.api.service.PhotoService;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;
import com.xiaonei.album.home.AlbumHome;
import com.xiaonei.album.model.Album;
import com.xiaonei.blog.home.BlogHome;
import com.xiaonei.page.service.FansService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.gossip.service.GossipService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.register.controllers.base.BaseRegAjaxController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.base.RegForm;

/**
 * <action path="/AjaxGetUserProfileInfo"
 * type="com.xiaonei.reg.register.action.init.ajax.GetUserProfileInfo" />
 * 
 * @author wangtai
 * 
 */
@Path("AjaxGetUserProfileInfo")
public class AjaxGetUserProfileInfoController extends BaseRegAjaxController {
    
    @Autowired
    GossipService gossipService;

	private static final String NO_VALUE = "GetUserProfileInfo_no_such_value";
	private static final int MAX_WAIT_TIMES = 5;
	private static final long SLEEP_TIME = 1l;
	private static final String Key_doing = "doing";
	private static final String Key_friend_num = "friend_num";
	private static final String Key_gossip_num = "gossip_num";
	private static final String Key_photo_num = "photo_num";
	private static final String Key_album_num = "album_num";
	private static final String Key_blog_num = "blog_num";
	private static final String Key_head_url = "head_url";
	private static final String Key_head_largeurl = "head_large_url";
	private static final String Key_page_num = "page_num";
	private static final String Key_page_headurl = "page_head_url";
	
	private static PhotoService photoService;
	private static AlbumListService albumListService;
	static{
        
         final ServiceFactory fact = ServiceFactories.getFactory();
         photoService = fact.getService(PhotoService.class);
         albumListService = fact.getService(AlbumListService.class);
    }
	
	@Override
	public RegForward executeRAt(RegMapping mapping, RegForm form,HttpServletRequest request, HttpServletResponse response) {
		String refer = request.getHeader("Referer");
		if (StringUtils.isEmpty(refer)) {
			return null;
		}
		final int userId;
		try {
			userId = Integer.valueOf(request.getParameter("id"));
		} catch (NumberFormatException e1) {
			return null;
		}
		final Map<String, String> ParameterMap = new ConcurrentHashMap<String, String>();
		Thread task1 = new Thread() {
			public void run() {
				List<Object> a = new ArrayList<Object>();
				Doing doingFromCache = null;
                try {
                    doingFromCache = DoingHome.getInstance().getDoingFromCache(userId);
                    if (null != doingFromCache) {
                        a.add(doingFromCache.getContent());
                        //20090814 sunji 将"getContent()"改成“getHtmlContentForHome()”，为了出表情
                        setPaInMap(ParameterMap, Key_doing, doingFromCache.getHtmlContent());
                    }
                } catch (RuntimeException e) {
                }
			}
		};
		taskManage(ParameterMap, task1, Key_doing);
		Thread task = new Thread() {
			public void run() {
				int length = 0;
				try {
				    List<Integer> fids = FriendsHome.getInstance().getFriendsIds(userId);
					length = fids!= null ? fids.size() : 0;
					setPaInMap(ParameterMap, Key_friend_num, length);
				} catch (RuntimeException e) {
				}
			}
		};
		taskManage(ParameterMap, task, Key_friend_num);
		Thread task2 = new Thread() {
			public void run() {
				try {
                    setPaInMap(ParameterMap, Key_gossip_num, gossipService.getGossipCount(userId));
                } catch (Exception e) {
                }
			}
		};
		taskManage(ParameterMap, task2, Key_gossip_num);
		Thread task3 = new Thread() {
			public void run() {
				try {
					//setPaInMap(ParameterMap, Key_photo_num, String.valueOf(AlbumHome.getInstance().getPhotoCount(userId)));	
				    setPaInMap(ParameterMap, Key_photo_num, String.valueOf(ServiceFutureHelper.execute(photoService.getPhotoCount(userId,userId),80)));
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		taskManage(ParameterMap, task3, Key_photo_num);
		Thread task4 = new Thread() {
			public void run() {
				try {
					setPaInMap(ParameterMap, Key_blog_num, String.valueOf(BlogHome.getInstance().getBlogCountForProfile(userId,99)));
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		taskManage(ParameterMap, task4, Key_blog_num);
		Thread task5 = new Thread() {
			public void run() {
				// url改从User中获取
				try {
                    User user = WUserAdapter.getInstance().get(userId);
                    setPaInMap(ParameterMap, Key_head_url, user.getHeadFullUrl());
                } catch (RuntimeException e) {
                }
			}
		};
		taskManage(ParameterMap, task5, Key_head_url);
		
		//取page数目相关
        Thread task6 = new Thread() {
            public void run() {
                try {
                    List<Integer> pageL = FansService.getInstance().getJoinPages(userId);
                    if(pageL!= null && pageL.size() > 0){
                        setPaInMap(ParameterMap, Key_page_num, pageL.size());
                    }
                } catch (RuntimeException e) {
                }
            }
        };
        taskManage(ParameterMap, task6, Key_page_num);
        
        //取page中的第一个头像
        Thread task7 = new Thread() {
            public void run() {
                try {
                    List<Integer> pageL = FansService.getInstance().getJoinPages(userId);
                    if(pageL!= null && pageL.size() > 0){
                        String pageHeadUrl = PageService.getInstance().get(pageL.get(0)).getHeadUrl();                        
                        setPaInMap(ParameterMap, Key_page_headurl, pageHeadUrl);
                    }
                } catch (RuntimeException e) {
                }
            }
        };
        taskManage(ParameterMap, task7, Key_page_headurl);
        
        Thread task8 = new Thread() {
            public void run() {
                try {
                    //get album general
                	//List<Album> al = AlbumHome.getInstance().getAlbumGeneral(userId); 
                	ServiceFuture<com.renren.ugc.model.album.Album[]> al = albumListService.getAlbumsPhotosLessCount(userId,userId,Album.LIMIT * 2);   
                    final com.renren.ugc.model.album.Album[] albums = ServiceFutureHelper.execute(al, 80);
                    setPaInMap(ParameterMap, Key_album_num, String.valueOf(al!=null ? albums.length : 0));
                    
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        taskManage(ParameterMap, task8, Key_album_num);
        
        Thread task9 = new Thread() {
            public void run() {
                // url改从User中获取
                try {
                    User user = WUserAdapter.getInstance().get(userId);
                    setPaInMap(ParameterMap, Key_head_largeurl, user.getLargeFullUrl());
                } catch (RuntimeException e) {
                }
            }
        };
        taskManage(ParameterMap, task9, Key_head_largeurl);
		return Print(ParameterMap, response);
	}

	private RegForward Print(Map<String, String> ParameterMap,
			ServletResponse response) {
		response.setContentType("text/html; charset=UTF-8");
		try {
			StringBuffer sb = new StringBuffer("{");
			for (Entry<String, String> entry : ParameterMap.entrySet()) {
				sb.append("\"" + entry.getKey() + "\":\"" + entry.getValue()
						+ "\",");
			}

			response.getWriter().print(
					sb.substring(0, sb.lastIndexOf(",")) + "}");
			response.getWriter().close();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (StringIndexOutOfBoundsException e) {
			return null;
		}
		return null;
	}

	private void taskManage(Map<String, String> ParameterMap,
			final Runnable taskDoing, String key) {
		RunnableTaskExecutor.getInstance().runTask(taskDoing);
		for (int i = 0; i < MAX_WAIT_TIMES; i++) {
			try {
				Thread.sleep(SLEEP_TIME);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			RegLogger.getLoggerWithType(this.getClass()).debug(
			        printMap(ParameterMap, key));
			if (!NO_VALUE.equals(getPaOutMap(ParameterMap, key))
					|| null != getPaOutMap(ParameterMap, key)) {
				break;
			}
			if ((i + 1) == MAX_WAIT_TIMES) {
				RegLogger.getLoggerWithType(this.getClass()).error(
						key + ": get failed");
			}
		}
	}

	private String printMap(Map<String, String> ParameterMap, String key) {
        return "[" + key + "]:::::[" + ParameterMap.get(key) + "]";
    }
	private String getPaOutMap(Map<String, String> ParameterMap, String key) {
		return ParameterMap.get(key);
	}

	private void setPaInMap(Map<String, String> ParameterMap, String key,
			Object value) {
		ParameterMap.put(key, NO_VALUE);
		String sValue = StringUtils.trimToEmpty(String.valueOf(value));
		ParameterMap.put(key, sValue);
	}

}

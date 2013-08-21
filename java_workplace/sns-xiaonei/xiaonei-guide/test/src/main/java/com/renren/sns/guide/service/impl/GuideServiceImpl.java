package com.renren.sns.guide.service.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.springframework.stereotype.Service;

import com.renren.app.share.IShareConstants;
import com.renren.app.share.hot.model.HotShare;
import com.renren.app.share.model.Share;
import com.renren.app.share.xoa.api.HotShareService;
import com.renren.app.share.xoa.api.ShareService;
import com.renren.sns.guide.logic.GuideLogic;
import com.renren.sns.guide.model.GuideCity;
import com.renren.sns.guide.service.GuideService;
import com.renren.sns.guide.utils.GuideMemCacheKeysPrefix;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.xiaonei.antispam.AntiSpamUtils;
import com.xiaonei.platform.component.tools.StringUtil;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.component.xfeed.publish.FeedBuilder;
import com.xiaonei.platform.component.xfeed.publish.expression.ExpressionFactory;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Service("guideService")
public class GuideServiceImpl implements GuideService {
	
	private MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide); ;
	
	@Override
	public Share parseHotShareToShare(HotShare hotShare) {
		ServiceFactory fact = ServiceFactories.getFactory();
		ShareService shareService = fact.getService(ShareService.class);
		GuideLogger.printLog("hotShare.getResourceUserId():"+hotShare.getResourceUserId());
		GuideLogger.printLog("hotShare.getUserId():"+hotShare.getUserId());
		GuideLogger.printLog("hotShare.getTitle():"+hotShare.getTitle());
		int ownerId = hotShare.getUserId();
		long shareId = hotShare.getShareId();
		ServiceFuture<Share> sf = shareService.getShare(ownerId, shareId).submit();
		Share share = sf.awaitUninterruptibly().getContent();		
		return share;
	}

	/**
	 * 发送热门新鲜事
	 */
	@Override
	public void transmitNewsFeed(User host) {
		if (host.getId() % 2 == 0) {
			ServiceFactory fact = ServiceFactories.getFactory();
			HotShareService hotShareService = fact.getService(HotShareService.class);
			int blogType = 1;
			int photoType = 8;
			int videoType = 10;
			int limit = 7;
			int stage = 10;
			ServiceFuture<HotShare[]> blogSf = (ServiceFuture<HotShare[]>) hotShareService.findHotShare(blogType, stage, 0, limit);
			blogSf.submit();			 
			HotShare[] blogList = (HotShare[])mcm.get(GuideMemCacheKeysPrefix.BLOG_HOT_SHARE);
			GuideLogger.printLog("blogList==null:"+(blogList==null));
			if(blogList==null){
				blogList = blogSf.awaitUninterruptibly().getContent();
				if(blogList!=null){
					GuideLogger.printLog("blogList.length:"+blogList.length);
					mcm.set(GuideMemCacheKeysPrefix.BLOG_HOT_SHARE, blogList,(int) MemCacheKeys.day * 3);
				}
			}				
			ServiceFuture<HotShare[]> photoSf = (ServiceFuture<HotShare[]>) hotShareService.findHotShare(photoType, stage, 0, limit);
			photoSf.submit();
			HotShare[] photoList = (HotShare[])mcm.get(GuideMemCacheKeysPrefix.PHOTO_HOT_SHARE);
			GuideLogger.printLog("photoList==null:"+(photoList==null));
			if(photoList==null){
				photoList = photoSf.awaitUninterruptibly().getContent();
				if(photoList!=null){
					GuideLogger.printLog("photoList.length:"+photoList.length);
					mcm.set(GuideMemCacheKeysPrefix.PHOTO_HOT_SHARE, photoList,(int) MemCacheKeys.day * 3);
				}
			}
			ServiceFuture<HotShare[]> videoSf = (ServiceFuture<HotShare[]>) hotShareService.findHotShare(videoType, stage, 0, limit);
			videoSf.submit();
			HotShare[] videoList = (HotShare[])mcm.get(GuideMemCacheKeysPrefix.VIDEO_HOT_SHARE);
			GuideLogger.printLog("videoList==null:"+(videoList==null));
			if(videoList==null){
				videoList = videoSf.awaitUninterruptibly().getContent();
				if(videoList!=null){
					GuideLogger.printLog("videoList.length:"+videoList.length);
					mcm.set(GuideMemCacheKeysPrefix.VIDEO_HOT_SHARE, videoList,(int) MemCacheKeys.day * 3);
				}
			}
			if(blogList!=null){
				GuideLogger.printLog("blogList.length:" + blogList.length);
				for(HotShare hs:blogList){
					Share share = parseHotShareToShare(hs);
					if(share==null){
						continue;
					}
					try {           
			            long blogId = share.getResourceId();
			            GuideLogger.printLog("share.getFromUserId():"+share.getFromUserId());
			            GuideLogger.printLog("share.getResourceUserId():"+share.getResourceUserId());
			            GuideLogger.printLog("share.getSourceUserId():"+share.getSourceUserId());	
			            GuideLogger.printLog("share.getUserId():"+share.getUserId());	
			            User resourceUser = SnsAdapterFactory.getUserAdapter().get(share.getResourceUserId());
			            FeedBuilder builder = new FeedBuilder.Builder(FeedDefinition.SHARE_BLOG, host.getId(), host.getName(), share.getId(), blogId)
			            	.action(FeedBuilder.ACTION.PUBLISH).merge(blogId).build();
			            builder.put("blogID", blogId);
			            builder.put("title", cutLongTitle(share.getTitle()));
			            builder.put("uID", resourceUser.getId());
			            builder.put("uName", resourceUser.getName());
			            builder.put("net", "");
			            builder.put("digest", cutLongTitle(share.getSummary()));
			            builder.put("url", share.getUrl());
			            builder.put("sID", share.getId());// 分享ID
			            builder.put("wap", 0);
			            try{
			                List<Integer> list = new ArrayList<Integer>();
			                list.add(host.getId());
			                Map<Integer,WUserCache> userCache = WUserCacheAdapter.getInstance().getUserCacheMap(list);
			                if(userCache.get(host.getId()).isCharity()){
			                    builder.put("isShowIcon","1");
			                }
			                builder.publish();
			                builder.setExpression(ExpressionFactory.getUser(host.getId()));
			            }
			            catch(Exception e){
			                e.printStackTrace();
			            }
			        } catch (Exception e) {
			            e.printStackTrace();
			        }
			        
				}
			}			
			if(photoList!=null){
				GuideLogger.printLog("photoList.length:" + photoList.length);
				for(HotShare hs:photoList){					
					Share share = parseHotShareToShare(hs);
					if(share==null){
						continue;
					}
					User resourceUser = SnsAdapterFactory.getUserAdapter().get(share.getResourceUserId());
					// 新
			        try {
			            long albumId = NumberUtils.toLong((String) share.getMetaMap().get(IShareConstants.META_KEY_TYPE2_ALBUM_ID));
			            if (albumId == 0) {
			                GuideLogger.printLog(String.format("数据错误，没有相册id：share=%d, user=%d", share.getId(), share.getUserId()));			                
			            }
			            String albumName = (String) share.getMetaMap().get(IShareConstants.META_KEY_TYPE2_ALBUM_NAME);
			            FeedBuilder builder = new FeedBuilder.Builder(FeedDefinition.SHARE_PHOTO, host.getId(),
			            		host.getName(), share.getId(), albumId).action(FeedBuilder.ACTION.PUBLISH).merge(albumId).build(); // 照片和相册合并?
			            builder.put("pUrl", share.getUrl());
			            builder.put("pSrc", share.getThumbFullUrl()); // 完整url
			            String title = abbreviate(share.getTitle(), 200);
			            builder.put("title", title != null ? title : "");
			            builder.put("aUrl", getAlbumUrl(albumId, resourceUser.getId()));
			            // 发新鲜事时给相册名中的特殊字符转义
			            if (StringUtils.containsAny(albumName, new char[] { '<', '>' })) {
			                albumName = AntiSpamUtils.getInstance().getSafePureContent(albumName);
			            }
			            builder.put("aName", albumName);
			            builder.put("uID", resourceUser.getId());
			            builder.put("uName", resourceUser.getName());
			            builder.put("aID", albumId);
			            builder.put("pID", share.getResourceId());
			            builder.put("sID", share.getId());// 分享ID
			            builder.put("wap", 0);
			            try {
			                List<Integer> list = new ArrayList<Integer>();
			                list.add(host.getId());
			                Map<Integer, WUserCache> userCache = WUserCacheAdapter.getInstance().getUserCacheMap(list);
			                if (userCache.get(host.getId()).isCharity()) {
			                    builder.put("isShowIcon", "1");
			                }
			                builder.publish();
			                builder.setExpression(ExpressionFactory.getUser(host.getId()));
			            } catch (Exception e) {
			                e.printStackTrace();
			            }			            
			        } catch (Exception e) {
			            e.printStackTrace();
			        }
				}
			}		
			if(videoList!=null){
				GuideLogger.printLog("videoList.length:" + videoList.length);
				for(HotShare hs:videoList){					
					Share share = parseHotShareToShare(hs);
					if(share==null){
						continue;
					}
					if (share.getMetaMap() != null && share.getMetaMap().get("fromConnect") != null && share.getMetaMap().get("fromConnect").equals("1")) {
						// Connect 发送新鲜事
						try {
							GuideLogger.printLog("dispatch connect 分享视频新鲜事");
							int appId = NumberUtils.toInt(String.valueOf(share.getMetaMap().get(IShareConstants.META_KEY_TYPE18_APP_ID)));
							if (appId > 0) {
								AppMenuItem app = AppMenuItemCacheMgr.getInstance().get(appId);
								long mergeID = getMergeId(share);
								FeedBuilder xbuilder = new FeedBuilder.Builder(FeedDefinition.XIAONEI_CONNECT_VIDEO, 
										host.getId(), host.getName(), share.getId(), share.getId()).merge(mergeID).build();
								xbuilder.put("appId", appId);
								xbuilder.put("pWhere", app.getAppName());
								xbuilder.put("sSrc", share.getUrl());
								xbuilder.put("headLine", share.getTitle());
								xbuilder.put("thumb", share.getThumbFullUrl());
								xbuilder.put("sID", String.valueOf(share.getId()));
								xbuilder.put("scale", "1.2");
								xbuilder.put("comm", cutLongTitle(share.getComment()));
								xbuilder.put("icon", app.getAppIcon());
								xbuilder.setResource(String.valueOf(getMergeId(share)));	
								xbuilder.publish();
								xbuilder.setExpression(ExpressionFactory.getUser(host.getId()));
							}
						} catch (Exception e) {
							e.printStackTrace();
						}
					} else {
						GuideLogger.printLog("dispatch 分享视频新鲜事");
						String scale = "1.2";
						try {
							// 计算缩放
							if (share.getMetaMap() != null) {
								Integer width = NumberUtils.toInt(share.getMetaMap().get(IShareConstants.META_KEY_TYPE10_WIDTH));
								Integer height = NumberUtils.toInt(share.getMetaMap().get(IShareConstants.META_KEY_TYPE10_HEIGHT));
								if (width != null && height != null) {
									float scaleValue = width / (float) height;
									scale = String.format("%.1f", scaleValue);
									GuideLogger.printLog(String.format("计算出视频缩放比例：width=%d, height=%d, scale=%s(%f)", width, height, scale, scaleValue));
								}
							}
						} catch (Exception e) {
							e.printStackTrace();
						}
						try {
							// 新
							int feedType = (int) FeedDefinition.SHARE_VIDEO;
							long parent = getMergeId(share);
							FeedBuilder builder = new FeedBuilder.Builder(feedType, share.getUserId(), host.getName(), 
									(long) share.getId(), parent).action(FeedBuilder.ACTION.PUBLISH).merge(parent).build();
							builder.put("url", share.getMetaMap() == null ? "" : share.getMetaMap().get(IShareConstants.META_KEY_TYPE10_FLASH_URL));
							builder.put("title", cutLongTitle(share.getTitle()));
							builder.put("scale", scale);
							builder.put("thumb",(StringUtils.isBlank(share.getThumbUrl())) ? IShareConstants.DEFAULT_TYPE10_THUBM_URL : share.getThumbFullUrl()); // 如果为空,传默认的图片
							builder.put("sID", share.getId());
							builder.put("new", "1");
							// 判断是否从firefox过去
							try {
								Map<String, String> metaMap = null;
								if ((metaMap = share.getMetaMap()) != null) {
									if (metaMap.get("browser") != null && metaMap.get("browser").equals(IShareConstants.BROWSER_TYPE_FIREFOX)) {
										GuideLogger.printLog("非 360 浏览器分享");
										builder.put("hText", "火狐浏览器");
										builder.put("hLink", "http://share.renren.com/share/help?ua=firefox");
									} else if (metaMap.get("browser") != null && metaMap.get("browser").equals(IShareConstants.BROWSER_TYPE_360)) {
										GuideLogger.printLog("360 浏览器分享");
										builder.put("hText", IShareConstants.BROWSER_HTEXT_360);
										builder.put("hLink", IShareConstants.BROWSER_HLINK_360);
									}
								}
								builder.publish();
								builder.setExpression(ExpressionFactory.getUser(host.getId()));
							} catch (Exception e) {
								e.printStackTrace();
							}							
						} catch (Exception e) {
							e.printStackTrace();							
						}
					}
				}
			}						
		} else {
			int middleSchoolId = 0;
			int enrollYear = 0;
			List<HighSchoolInfo> hsInfoList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(host.getId());
			if (hsInfoList != null) {
				for (HighSchoolInfo hsi : hsInfoList) {
					if (hsi.getEnrollYear() > enrollYear) {
						enrollYear = hsi.getEnrollYear();
						middleSchoolId = hsi.getHighSchoolId();
					}
				}
			}
			if (enrollYear == 0) {
				JuniorHighSchoolInfo junInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(host.getId());
				if (junInfo != null) {
					middleSchoolId = junInfo.getJunHighentarySchoolId();
					enrollYear = junInfo.getJunHighentarySchoolYear();
				}
			}
			int cityStarLimit = 8;
			GuideCity gc = GuideLogic.getInstance().getCityCodeByMiddleSchoolId(middleSchoolId);
			//有可能根据学校取不到城市
			if(gc!=null){
				String cityCode = gc.getCityCode();
				List<Integer> idList = GuideLogic.getInstance().getUserStarIdsByCityCode(cityCode,cityStarLimit);
				GuideLogger.printLog("city star cityCode:"+cityCode);
				GuideLogger.printLog("city star list:"+idList.size());
				GuideLogic.getInstance().guideBuddyReplyNotify(host.getId(), idList);
			}			
		}		
	}
	
	/**
     * 截短长标题(评论)
     * 
     * @param title
     * @return
     */
	@Override
    public String cutLongTitle(String title) {
        if (title != null) {
            title = StringUtil.cutLongEngWord(title, 25);
        }
        return title;
    }
    
    /**
     * 截短字符串
     * 
     * @param str
     * @return
     */
	@Override
	public String abbreviate(String str, int len) {
        if (str != null) {
            return StringUtil.cutLongEngWord(str, len);
        }
        return str;
    }
    
	@Override
    public String getAlbumUrl(long albumId, int userId) {
        return String.format("%s/photo/%d/album-%d", OpiConstants.urlPhoto, userId, albumId);
    }
    
    /**
     * 计算新鲜事合并id
     * 
     * @param link
     * @return
     */
	@Override
    public long getMergeId(Share share) {
        if (share.getResourceId() > 0) {
            return share.getResourceId();
        }
        int hashCode = share.getUrl().hashCode();
        if(hashCode == Integer.MIN_VALUE){
        	return Integer.MIN_VALUE;
        }
		return Math.abs(hashCode);
    }
	public static void main(String[] args) {
		int a = Integer.MIN_VALUE;
		System.out.println(Math.abs(a));
	}

}

package com.xiaonei.platform.core.opt;

import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.ServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;


/**
 * @author Administrator
 * 
 */
public class OpiConstants {

	private static boolean reduceDns = true;

	private static boolean mergeHomeProfile = true;

	private static Log logger = LogFactory.getLog(OpiConstants.class);

	private static final Map<String, Properties> domainIdentified = new ConcurrentHashMap<String, Properties>();

	// private static final ThreadLocal<ServletRequest> requests = new
	// ThreadLocal<ServletRequest>();

	private static final ThreadLocal<String> domains = new ThreadLocal<String>();

	// /**
	// * 请改为使用 {@link #setCurrentDomain(String)}
	// *
	// * @param request
	// */
	// @Deprecated
	// public static void setRequest(ServletRequest request) {
	// requests.set(request);
	// domains.set(findDomain(request));
	// }

	// /**
	// * 请改为使用 {@link #getCurrentDomain()}
	// *
	// * @return
	// */
	// @Deprecated
	// public static HttpServletRequest getRequest() {
	// return (HttpServletRequest) requests.get();
	// }

	/**
	 * 返回当前请求的域名(renren.com、kaixin.com)
	 * 
	 * @return
	 */
	public static String getCurrentDomain() {
		return domains.get();
	}

	/**
	 * 设置当前使用的域名
	 * 
	 * @param domain
	 */
	public static void setCurrentDomain(String domain) {
		domains.set(domain);
	}

	public static void clearCurrentDomain() {
		domains.remove();
	}

	/**
	 * 增加或更新域名所使用的专有文本信息
	 * 
	 * @param domain
	 * @param p
	 */
	public static void addDomainIdentifiedProperties(String domain, Properties p) {
		domainIdentified.put(domain, p);
	}

	/**
	 * 从给定的请求对象中解析出其域名出来
	 * 
	 * @param request
	 * @return
	 */
	public static String findDomain(ServletRequest request) {
		if (request == null) {
			return null;
		}
		String domain;
		String serverName = request.getServerName();
		int dot = serverName.lastIndexOf(".");
		if (dot != -1) {
			dot = serverName.lastIndexOf(".", dot - 1);
			domain = serverName.substring(dot + 1);
		} else {
			domain = serverName;
		}
		domain = domain.toLowerCase();
		if (!"renren.com".equals(domain) && !"kaixin.com".equals(domain)) {
			domain = "renren.com";
		}
		return domain;
	}

	public static Properties getCurrentDomainIdentifiedProperties() {
		return (Properties) domainIdentified.get(domains.get());
	}

	/**
	 * 根据当前线程绑定的请求对象，获取在domainIdentified.renren.com.xml/
	 * domainIdentifiedkaixin.com.xml相应key的value串
	 * 
	 * @param key
	 * @return
	 */
	public static String getDomainIdentifiedProperty(String key) {
		String value = getDomainIdentifiedProperty(key, key);
		return value;
	}

	/**
	 * 根据当前线程绑定的请求对象，获取在domainIdentified.renren.com.xml/
	 * domainIdentifiedkaixin.com.xml相应key的value串,如果没有定义相应的key时,则返回defValue
	 * 
	 * @param key
	 * @param defValue
	 * @return
	 */
	public static String getDomainIdentifiedProperty(String key, String defValue) {
		String domain = getCurrentDomain();
		if (domain == null) {
			logger.warn("not found current domain");
			return defValue;
		}
		Properties p = (Properties) domainIdentified.get(domain);
		if (p == null) {
			logger.error("not found properites for " + domain);
			logger.error("properties keys: "
					+ domainIdentified.keySet().toArray(new String[0]));
			return defValue;
		}
		if (defValue == null) {
			return p.getProperty(key);
		}
		return p.getProperty(key, defValue);
	}

	/**
	 * 根据当前线程绑定的请求对象，获取在domainIdentified.renren.com.xml/ domainIdentifiedkaixin
	 * .com.xml相应key的value串，并使用所提供的参数替换value串中的格式符号(e.g. %s)
	 * 
	 * @param key
	 * @param args
	 * @return
	 */
	public static String formatDomainIdentifiedProperty(String key,
			Object... args) {
		String value = getDomainIdentifiedProperty(key);
		if (value == null) {
			return value;
		}
		return String.format(value, args);
	}

	// private static String defaultDomain;

	private static String getDefaultDomain() {
		return defXiaoneiDomain;
		/*
		 * if (defaultDomain == null) { String mDomain = null; try { mDomain =
		 * CoreConfigDAO.getInstance().getCoreConfigValue(keyDefaultDomain); }
		 * catch (Throwable e) { e.printStackTrace(); } if (mDomain == null ||
		 * mDomain.equals("")) { //没有取到，基本不会发生 try { if (IPAddress.isXiaonei())
		 * { //用中间层的工具方法判断，双保险 mDomain = DOMAIN_XIAONEI; } else { mDomain =
		 * DOMAIN_KAIXIN; } } catch (Throwable e) { e.printStackTrace(); } }
		 * 
		 * if (mDomain != null && mDomain.indexOf(DOMAIN_XIAONEI) >= 0) { //is
		 * xiaonei defaultDomain = defXiaoneiDomain; } else { //is kaixin
		 * defaultDomain = defKaixinDomain; }
		 * 
		 * } return defaultDomain;
		 */
	}

	/**
	 * 域名
	 */
	public static DyString domain = new DyString() {

		@Override
		public String toString() {
			String domain = getCurrentDomain();
			if (domain == null) {
				return getDefaultDomain();
			}
			return domain;
		}
	};

	/**
	 * 四个默认做参考的域名
	 */
	public static final String defXiaoneiDomain = "renren.com";

	private static final String defXiaoneiStatic = "a.xnimg.cn";

	public static final String defKaixinDomain = "kaixin.com";

	// private static String defKaixinStatic = "rrimg.com";

	// private static String DOMAIN_XIAONEI = "renren";

	// private static String DOMAIN_KAIXIN = "kaixin";

	//
	// public static final DyString domainStatic = new DyString() {
	//
	// @Override
	// public String toString() {
	// if (domain.toString().equals("renren.com")) {
	// return defXiaoneiStatic;
	// } else {
	// return defKaixinStatic;
	// }
	// }
	// };

	public static final String domainStatic = defXiaoneiStatic;

	// static{
	// String mDomain = null;
	// try {
	// mDomain =
	// CoreConfigDAO.getInstance().getCoreConfigValue(keyDefaultDomain);
	// } catch (Throwable e) {
	// e.printStackTrace();
	// }
	// if (mDomain == null || mDomain.equals("")) { //没有取到，基本不会发生
	// try {
	// if (IPAddress.isXiaonei()) { //用中间层的工具方法判断，双保险
	// mDomain = DOMAIN_XIAONEI;
	// } else {
	// mDomain = DOMAIN_KAIXIN;
	// }
	// } catch (Throwable e) {
	// e.printStackTrace();
	// }
	// }
	//
	// if (mDomain != null && mDomain.indexOf(DOMAIN_XIAONEI) >= 0) {
	// //is xiaonei
	// domain = defXiaoneiDomain;
	// domainStatic = defXiaoneiStatic;
	// } else {
	// //is kaixin
	// domain = defKaixinDomain;
	// domainStatic = defKaixinStatic;
	// }
	// }

	/**
	 * @return 是否是校内
	 */
	public static boolean isXiaonei() {
		return defXiaoneiDomain.equals(domain.toString());
	}

	/**
	 * @return 是否是开心
	 */
	public static boolean isKaixin() {
		return !isXiaonei();
	}

	/**
	 * 带有http://和域名
	 */
	public static final DyString urlMain = new DyString() {

		@Override
		public String toString() {
			if (mergeHomeProfile)
				return urlWww.toString();
			else
				return "http://" + domain;
		}

	};

	public static final DyString urlProfile = new DyString() {

		@Override
		public String toString() {
			if (mergeHomeProfile)
				return urlWww.toString();
			else
				return "http://" + domain;
		}
	};

	public static final DyString domainMain = domain;

	public static final String domainSmallStatic = "s.xnimg.cn";

	public static final DyString domainPassport = new DyString() {

		@Override
		public String toString() {
			if (reduceDns) {
				return "www.renren.com";
			} else {
				return "passport.renren.com";
			}
		}
	};

	public static final DyString domainClass = new DyString() {

		@Override
		public String toString() {
			return "class." + domain;
		}
	};

	public static final DyString domainClub = new DyString() {

		@Override
		public String toString() {
			return "club." + domain;
		}
	};

	public static final DyString domainMobileApps = new DyString() {

		@Override
		public String toString() {
			return "mapps." + domain;
		}
	}; // wap上的apps服务domain

	public static final DyString urlWww = new DyString() {

		@Override
		public String toString() {
			return "http://www." + domain;
		}
	};

	public static final DyString urlLogin = new DyString() {

		@Override
		public String toString() {
			if (reduceDns) {
				return urlWww.toString();
			} else {
				return "http://login." + domain;
			}
		}
	};

	public static final DyString urlHome = new DyString() {

		@Override
		public String toString() {
			if (reduceDns) {
				return urlWww.toString();
			} else {
				return "http://home." + domain;
			}
		}
	};

	// 校内更名人人后，头像服务的域名还没换，所以这么搞一下
	// static {
	// String tempDomain = domain;
	// if (isXiaonei()) {
	// tempDomain = "xiaonei.com";
	// };
	// urlHead = "http://head." + tempDomain;
	// urlTinyHead = "http://tiny.head." + tempDomain;
	// urlImg = "http://img." + tempDomain;
	// };

	// public static final DyString urlHead = new DyString() {
	//
	// @Override
	// public String toString() {
	// return "http://head." + domain;
	// }
	// };

	public static final String urlHead = "http://head.xiaonei.com";

	// public static final DyString urlTinyHead = new DyString() {
	//
	// @Override
	// public String toString() {
	// return "http://tiny.head." + domain;
	// }
	// };

	public static final String urlTinyHead = "http://tiny.head.xiaonei.com";

	// public static final DyString urlImg = new DyString() {
	//
	// @Override
	// public String toString() {
	// return "http://img." + domain;
	// }
	// };

	public static final String urlImg = "http://img.xiaonei.com";

	// public static final String urlPassport = "http://passport.renren.com";

	public static final DyString urlPassport = new DyString() {

		@Override
		public String toString() {
			if (reduceDns) {
				return "http://www.renren.com";
			} else {
				return "http://passport.renren.com";
			}
		}
	};

	public static final String urlPic001 = "http://pic.xiaonei.com";

	public static final String urlPic002 = "http://pic2.xiaonei.com";

	public static final DyString urlHeadUpload = new DyString() {

		@Override
		public String toString() {
			return "http://head.upload." + domain;
		}
	};

	public static final DyString urlHeadUpload2 = new DyString() {

		@Override
		public String toString() {
			return "http://head2.upload." + domain;
		}
	};

	public static final DyString urlFriend = new DyString() {

		@Override
		public String toString() {
			return "http://friend." + domain;
		}
	};

	public static final DyString urlClass = new DyString() {

		@Override
		public String toString() {
			return "http://class." + domain;
		}
	};

	public static final DyString urlTeam = new DyString() {

		@Override
		public String toString() {
			return "http://team." + domain;
		}
	};

	public static final DyString urlGroup = new DyString() {

		@Override
		public String toString() {
			return "http://group." + domain;
		}
	};

	public static final DyString urlTieba = new DyString() {

		@Override
		public String toString() {
			return "http://tieba." + domain;
		}
	};

	public static final DyString urlNetwork = new DyString() {

		@Override
		public String toString() {
			return "http://network." + domain;
		}
	};

	public static final DyString urlEvent = new DyString() {

		@Override
		public String toString() {
			return "http://event." + domain;
		}
	};

	public static final DyString urlCourse = new DyString() {

		@Override
		public String toString() {
			return "http://course." + domain;
		}
	};

	public static final DyString urlMarket = new DyString() {

		@Override
		public String toString() {
			return "http://market." + domain;
		}
	};

	public static final DyString urlMovie = new DyString() {

		@Override
		public String toString() {
			return "http://movie." + domain;
		}
	};

	public static final DyString urlAbc = new DyString() {

		@Override
		public String toString() {
			return "http://abc." + domain;
		}
	};//

	public static final DyString urlClub = new DyString() {

		@Override
		public String toString() {
			return "http://club." + domain;
		}
	};

	public static final DyString urlBlog = new DyString() {

		@Override
		public String toString() {
			return "http://blog." + domain;
		}
	};

	public static final DyString urlPhoto = new DyString() {

		@Override
		public String toString() {
			return "http://photo." + domain;
		}
	};

	public static final DyString urlMsg = new DyString() {

		@Override
		public String toString() {
			return "http://msg." + domain;
		}
	};

	public static final DyString urlStatus = new DyString() {

		@Override
		public String toString() {
			return "http://status." + domain;
		}
	};

	public static final DyString urlUpload = new DyString() {

		@Override
		public String toString() {
			return "http://upload." + domain;
		}
	};

	public static final DyString urlUpload2 = new DyString() {

		@Override
		public String toString() {
			return "http://upload2." + domain;
		}
	};

	public static final DyString urlSchool = new DyString() {

		@Override
		public String toString() {
			return "http://school." + domain;
		}
	};

	public static final DyString urlBrowse = new DyString() {

		@Override
		public String toString() {
			return "http://browse." + domain;
		}
	};

	public static final DyString urlAdmin = new DyString() {

		@Override
		public String toString() {
			return "http://admin." + domain;
		}
	};

	public static final DyString urlSupport = new DyString() {

		@Override
		public String toString() {
			return "http://support." + domain;
		}
	};

	public static final String urlStatic = "http://" + domainStatic;

	public static final String urlSmallStatic = "http://" + domainSmallStatic;

	public static final DyString urlSource = new DyString() {

		@Override
		public String toString() {
			return "http://source." + domain;
		}
	};

	public static final String urlPhotoAlbum = "http://photos.album.5q.com";

	public static final String urlPhotoGroup = "http://photos.group.5q.com";

	public static final String urlPhoto2Album = "http://photos2.album.5q.com";

	public static final String urlPhoto2Group = "http://photos2.group.5q.com";

	public static final DyString urlReg = new DyString() {

		@Override
		public String toString() {
			return "http://reg." + domain;
		}
	};

	public static final DyString urlInvite = new DyString() {

		@Override
		public String toString() {
			return "http://invite." + domain;
		}
	};

	public static final DyString urlGuide = new DyString() {

		@Override
		public String toString() {
			return "http://guide." + domain;
		}
	};

	public static final DyString urlGoto = new DyString() {

		@Override
		public String toString() {
			return "http://goto." + domain;
		}
	}; // 分享跳转

	public static final DyString urlApp = new DyString() {

		@Override
		public String toString() {
			return "http://app." + domain;
		}
	};

	public static final DyString urlApps = new DyString() {

		@Override
		public String toString() {
			return "http://apps." + domain;
		}
	};

	public static final DyString urlApi = new DyString() {

		@Override
		public String toString() {
			return "http://api." + domain;
		}
	};

	public static final DyString urlDev = new DyString() {

		@Override
		public String toString() {
			return "http://dev." + domain;
		}
	};

	public static final DyString urlSurvey = new DyString() {

		@Override
		public String toString() {
			return "http://survey." + domain;
		}
	};// 调查

	public static final DyString urlShare = new DyString() {

		@Override
		public String toString() {
			return "http://share." + domain;
		}
	};//

	public static final DyString urlPoll = new DyString() {

		@Override
		public String toString() {
			return "http://poll." + domain;
		}
	};//

	public static final DyString urlI = new DyString() {

		@Override
		public String toString() {
			return "http://i." + domain;
		}
	}; // vip

	public static final DyString urlGift = new DyString() {

		@Override
		public String toString() {
			return "http://gift." + domain;
		}
	};// 礼物

	public static final DyString urlUploadI = new DyString() {

		@Override
		public String toString() {
			return "http://upload.i." + domain;
		}
	}; // vip上传

	public static final DyString urlPay = new DyString() {

		@Override
		public String toString() {
			return "http://pay." + domain;
		}
	}; // 支付

	public static final String urlGiftImg = urlStatic + "/gift";

	public static final DyString urlGg = new DyString() {

		@Override
		public String toString() {
			return "http://gg." + domain;
		}
	};//

	public static final DyString urlDog = new DyString() {

		@Override
		public String toString() {
			return "http://dog." + domain;
		}
	};//

	public static final DyString urlWpi = new DyString() {

		@Override
		public String toString() {
			return "http://wpi." + domain;
		}
	};//

	public static final DyString urlXyx = new DyString() {

		@Override
		public String toString() {
			return "http://xyx." + domain;
		}
	};//

	public static final DyString urlDoing = new DyString() {

		@Override
		public String toString() {
			return "http://status." + domain;
		}
	};//

	public static final DyString urlRegJump = new DyString() {

		@Override
		public String toString() {
			return "http://wwv." + domain;
		}
	}; // 注册跳转的一个url，2009-03-06添加，by Li Weibo

	public static final DyString urlMobilePay = new DyString() {

		@Override
		public String toString() {
			return "http://mpay." + domain;
		}
	};

	public static final DyString urlMobilePortal = new DyString() {

		@Override
		public String toString() {
			return "http://mobile." + domain;
		}
	};

	public static final DyString urlMobile = new DyString() {

		@Override
		public String toString() {
			return "http://" + domainMobile;
		}
	};

	public static final DyString urlMobileApp = new DyString() {

		@Override
		public String toString() {
			return "http://mapp." + domain;
		}
	};

	public static final DyString urlMobileApps = new DyString() {

		@Override
		public String toString() {
			return "http://" + domainMobileApps;
		}
	};

	public static final DyString urlPage = new DyString() {

		@Override
		public String toString() {
			return "http://page." + domain;
		}
	};

	/**
	 * 情侣空间使用的域名
	 */
	public static final DyString urlLover = new DyString() {

		@Override
		public String toString() {
			return "http://lover." + domain;
		}
	};
	
	public static final DyString urlReq = new DyString() {

		@Override
		public String toString() {
			return "http://req." + domain;
		}
	};

	public static final DyString urlIcode = new DyString() {

		@Override
		public String toString() {
			return "http://icode." + domain;
		}
	};

	public static final DyString urlGame = new DyString() {

		@Override
		public String toString() {
			return "http://game." + domain;
		}
	};

	/**
	 * 问答
	 */
	public static final DyString urlWenda = new DyString() {

		@Override
		public String toString() {
			return "http://wenda." + domain;
		}
	};

	public static final String loginKey_session = "hostid";

	public static final String JEBE_COOKIE_NAME = "jebecookies";

	// #由于原先5q的静态信息域名与校内 img.xiaonei.com 域名冲突 所以5q的静态服务域名改用 txt.5q.com
	public static final String urlTxt5q = "http://txt.5q.com";

	public static final DyString domainMobile = new DyString() {

		@Override
		public String toString() {
			String domainStr = domain.toString();
			if ("renren.com".equals(domainStr)
					|| "xiaonei.com".equals(domainStr)) {
				return "3g.renren.com";
			}
			return "m." + domain;
		}
	};

	public static final String userMainUrl = "0/0/main.jpg";

	public static final String userHeadUrl = "0/0/head.jpg";

	public static final String userTinyUrl = "0/0/tiny.jpg";

	public static final String userLargeUrl = "0/0/large.jpg";

	public static final String userMenTinyUrl = "0/0/men_tiny.gif";

	public static final String userWomenTinyUrl = "0/0/women_tiny.gif";

	public static final String userMenHeadUrl = "0/0/men_head.gif";

	public static final String userWomenHeadUrl = "0/0/women_head.gif";

	public static final String userMenMainUrl = "0/0/men_main.gif";

	public static final String userWomenMainUrl = "0/0/women_main.gif";

	public static final DyString cookieDomain = new DyString() {

		@Override
		public String toString() {
			return "." + domain;
		}
	};

	public static final String tempPath = "/data/temp";

	public static final String tempHeadPath = tempPath + "/" + "head";

	public static final String tempPhotoPath = tempPath + "/" + "photo";

	public static final String tempBlogPath = tempPath + "/" + "blog";

	public static final String tempPic001Path = tempPath + "/" + "pic001";

	public static final String tempTribePath = tempPath + "/" + "tribe";

	public static final String tempShoolMatePath = tempPath + "/"
			+ "schoolmate";

	public static final String tempTmpPath = tempPath + "/" + "tmp";

	public static final String forwardUploadError = "errorupload";

	public static final String forwardNoPermission = "nopermission";

	public static final String forwardErrorNotExist = "errornotexist";

	public static final String forwardErrorRegist = "errorregister";

	public static final String forwardError = "error";

	public static final String defaultEncode = "UTF-8";

	// 安全中心
	public static final DyString keySafeCenterDisabled = new DyString() {

		@Override
		public String toString() {
			return "xiaonei.safecenter.disabled";
		}
	};

	// 安全中心
	public static final DyString urlSafeCenter = new DyString() {

		@Override
		public String toString() {
			return "http://safe." + domain;
		}
	};

	// 俱乐部(公共主页团队维护)
	public static final DyString urlOrg = new DyString() {

		@Override
		public String toString() {
			return "http://org.renren.com";
		}
	};

	// 留言板(吕恩乐)
	public static final DyString domainGossip = new DyString() {

		@Override
		public String toString() {
			return "gossip." + domain;
		}
	};

	public static final DyString urlGossip = new DyString() {

		@Override
		public String toString() {
			return "http://" + domainGossip;
		}
	};

	public static final DyString domainWidget = new DyString() {

		@Override
		public String toString() {
			return "widget.renren.com";
			// return "widget." + domain;
		}
	};

	public static final DyString urlWidget = new DyString() {

		@Override
		public String toString() {
			return "http://widget.renren.com";
			// return "http://widget." + domain;
		}
	};

	public static final DyString domainWorkflow = new DyString() {

		@Override
		public String toString() {
			return "workflow.renren.com";
			// return "workflow." + domain;
		}
	};

	public static final DyString urlWorkflow = new DyString() {

		@Override
		public String toString() {
			return "http://workflow.renren.com";
			// return "http://workflow." + domain;
		}
	};
	
	public static final DyString domainXiaozu = new DyString() {

		@Override
		public String toString() {
			return "xiaozu." + domain;
		}
	};
	public static final DyString urlXiaozu = new DyString() {

		@Override
		public String toString() {
			return "http://" + domainXiaozu;
		}
	};
	
	
	public static final DyString domainZhan = new DyString() {

		@Override
		public String toString() {
			return "zhan." + domain;
		}
	};
	public static final DyString urlZhan = new DyString() {

		@Override
		public String toString() {
			return "http://" + domainZhan;
		}
	};
	
	public static final DyString domainTopic = new DyString() {

		@Override
		public String toString() {
			return "topic." + domain;
		}
	};
	public static final DyString urlTopic = new DyString() {

		@Override
		public String toString() {
			return "http://" + domainTopic;
		}
	};
	
	/**
	 * UGC的minigroup项目用的域名
	 */
	public static final DyString domainQun = new DyString() {

		@Override
		public String toString() {
			return "qun." + domain;
		}
	};
	public static final DyString urlQun = new DyString() {

		@Override
		public String toString() {
			return "http://" + domainQun;
		}
	};

}

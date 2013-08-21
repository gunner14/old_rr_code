package com.xiaonei.platform.core.opt.base.action;

import java.lang.reflect.Field;
import java.net.InetAddress;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.DyString;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 这个监听器做2件事，<br/>
 * 1、将机器名设置到context中，这样查看版本的时候可以知道是哪台机器
 * 2、将常用的域名（如：http://www.xiaonei.com）也放到context中。 应用启动监听器<br>
 * 初始化一些全局的属性
 * 
 * @author lifs
 */
public class InitiateListener implements ServletContextListener {

    public static Log logger = LogFactory.getLog(InitiateListener.class);

    public void contextInitialized(ServletContextEvent arg0) {
        //页脚的版本号显示
        String name = null;
        try {
            InetAddress hostIP = InetAddress.getLocalHost();
            name = hostIP.getHostName();
            arg0.getServletContext().setAttribute("hostName", name);
            logger.info("---------------- ServletContext().setAttribute(hostName)=" + name);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        /*arg0.getServletContext().setAttribute("urlApp", OpiConstants.urlApp);
        arg0.getServletContext().setAttribute("urlApps", OpiConstants.urlApps);
        arg0.getServletContext().setAttribute("urlApi", OpiConstants.urlApi);
        arg0.getServletContext().setAttribute("urlDev", OpiConstants.urlDev);
        arg0.getServletContext().setAttribute("urlWww", OpiConstants.urlWww);
        arg0.getServletContext().setAttribute("urlFriend", OpiConstants.urlFriend);
        arg0.getServletContext().setAttribute("urlClass", OpiConstants.urlClass);
        arg0.getServletContext().setAttribute("urlTeam", OpiConstants.urlTeam);
        arg0.getServletContext().setAttribute("urlMain", OpiConstants.urlMain);
        arg0.getServletContext().setAttribute("urlProfile", OpiConstants.urlProfile);
        arg0.getServletContext().setAttribute("urlSearch", OpiConstants.urlBrowse);
        arg0.getServletContext().setAttribute("urlUpload", OpiConstants.urlUpload);
        arg0.getServletContext().setAttribute("urlUpload2", OpiConstants.urlUpload2);
        arg0.getServletContext().setAttribute("urlLogin", OpiConstants.urlLogin);
        arg0.getServletContext().setAttribute("urlGroup", OpiConstants.urlGroup);
        arg0.getServletContext().setAttribute("urlTieba", OpiConstants.urlTieba);
        arg0.getServletContext().setAttribute("urlNetwork", OpiConstants.urlNetwork);
        arg0.getServletContext().setAttribute("urlEvent", OpiConstants.urlEvent);
        arg0.getServletContext().setAttribute("urlCourse", OpiConstants.urlCourse);
        arg0.getServletContext().setAttribute("urlMarket", OpiConstants.urlMarket);
        arg0.getServletContext().setAttribute("urlMovie", OpiConstants.urlMovie);
        arg0.getServletContext().setAttribute("urlAbc", OpiConstants.urlAbc);
        arg0.getServletContext().setAttribute("urlClub", OpiConstants.urlClub);
        arg0.getServletContext().setAttribute("urlBlog", OpiConstants.urlBlog);
        arg0.getServletContext().setAttribute("urlPhoto", OpiConstants.urlPhoto);
        arg0.getServletContext().setAttribute("urlShare", OpiConstants.urlShare);
        arg0.getServletContext().setAttribute("urlMsg", OpiConstants.urlMsg);
        arg0.getServletContext().setAttribute("urlStatus", OpiConstants.urlStatus);
        arg0.getServletContext().setAttribute("urlHome", OpiConstants.urlHome);
        arg0.getServletContext().setAttribute("urlReg", OpiConstants.urlReg);
        arg0.getServletContext().setAttribute("urlBrowse", OpiConstants.urlBrowse);
        arg0.getServletContext().setAttribute("urlSchool", OpiConstants.urlSchool);
        arg0.getServletContext().setAttribute("urlImg", OpiConstants.urlImg);
        arg0.getServletContext().setAttribute("urlHead", OpiConstants.urlHead);
        arg0.getServletContext().setAttribute("urlTxt5q", OpiConstants.urlTxt5q);
        arg0.getServletContext().setAttribute("urlStatic", OpiConstants.urlStatic);
        arg0.getServletContext().setAttribute("urlSmallStatic", OpiConstants.urlSmallStatic);
        arg0.getServletContext().setAttribute("urlSupport", OpiConstants.urlSupport);
        arg0.getServletContext().setAttribute("urlPhotoAlbum", OpiConstants.urlPhotoAlbum);
        arg0.getServletContext().setAttribute("urlPhotoGroup", OpiConstants.urlPhotoGroup);
        arg0.getServletContext().setAttribute("urlPhoto2Album", OpiConstants.urlPhoto2Album);
        arg0.getServletContext().setAttribute("urlPhoto2Group", OpiConstants.urlPhoto2Group);
        arg0.getServletContext().setAttribute("urlGuide", OpiConstants.urlGuide);
        arg0.getServletContext().setAttribute("urlGoto", OpiConstants.urlGoto);
        arg0.getServletContext().setAttribute("urlAdmin", OpiConstants.urlAdmin);
        arg0.getServletContext().setAttribute("urlInvite", OpiConstants.urlInvite);
        arg0.getServletContext().setAttribute("urlHeadUpload", OpiConstants.urlHeadUpload);
        arg0.getServletContext().setAttribute("urlHeadUpload2", OpiConstants.urlHeadUpload2);
        arg0.getServletContext().setAttribute("urlMobile", OpiConstants.urlMobile);
        arg0.getServletContext().setAttribute("urlPoll", OpiConstants.urlPoll);
        arg0.getServletContext().setAttribute("urlWpi", OpiConstants.urlWpi);//webpager
        arg0.getServletContext().setAttribute("urlDoing", OpiConstants.urlDoing);
        arg0.getServletContext().setAttribute("urlRegJump", OpiConstants.urlRegJump); //注册跳转的一个url，2009-03-06添加，by Li Weibo
        arg0.getServletContext().setAttribute("urlMobilePay", OpiConstants.urlMobilePay);
        arg0.getServletContext().setAttribute("urlMobilePortal", OpiConstants.urlMobilePortal);
        arg0.getServletContext().setAttribute("urlMobileApp", OpiConstants.urlMobileApp);
        arg0.getServletContext().setAttribute("urlMobileApps", OpiConstants.urlMobileApps);
        arg0.getServletContext().setAttribute("urlPage", OpiConstants.urlPage);
        arg0.getServletContext().setAttribute("urlReq", OpiConstants.urlReq);
        arg0.getServletContext().setAttribute("urlI", OpiConstants.urlI);
        arg0.getServletContext().setAttribute("urlGift", OpiConstants.urlGift);
        arg0.getServletContext().setAttribute("urlUploadI", OpiConstants.urlUploadI);
        arg0.getServletContext().setAttribute("urlPay", OpiConstants.urlPay);
        arg0.getServletContext().setAttribute("urlIcode", OpiConstants.urlIcode);
        arg0.getServletContext().setAttribute("urlGiftImg", OpiConstants.urlGiftImg);
        arg0.getServletContext().setAttribute("urlDog", OpiConstants.urlDog);
        arg0.getServletContext().setAttribute("urlXyx", OpiConstants.urlXyx);
        arg0.getServletContext().setAttribute("urlGame", OpiConstants.urlGame);
        arg0.getServletContext().setAttribute("urlGg", OpiConstants.urlGg);
        arg0.getServletContext().setAttribute("urlWenda", OpiConstants.urlWenda);
        arg0.getServletContext().setAttribute("urlSafeCenter", OpiConstants.urlSafeCenter);
        arg0.getServletContext().setAttribute("urlOrg", OpiConstants.urlOrg);
        arg0.getServletContext().setAttribute("urlGossip", OpiConstants.urlGossip);
        arg0.getServletContext().setAttribute("urlPassport", OpiConstants.urlPassport);
        arg0.getServletContext().setAttribute("urlWidget", OpiConstants.urlWidget);
        arg0.getServletContext().setAttribute("urlWorkflow", OpiConstants.urlWorkflow);
        arg0.getServletContext().setAttribute("urlLover", OpiConstants.urlLover);*/
        
        registerUrls(arg0.getServletContext());	//自动注册各种url常量
        
        //页面用到的完整域名（去掉http://头的），如www.xiaonei.com
        arg0.getServletContext().setAttribute("sysdomain", OpiConstants.domain);
        arg0.getServletContext().setAttribute("domainClub", OpiConstants.domainClub);
        arg0.getServletContext().setAttribute("domainPassport", OpiConstants.domainPassport);
        arg0.getServletContext().setAttribute("domainClass", OpiConstants.domainClass);
        arg0.getServletContext().setAttribute("cookieDomain", OpiConstants.cookieDomain);
        arg0.getServletContext().setAttribute("domain", OpiConstants.domain);
        arg0.getServletContext().setAttribute("domainMain", OpiConstants.domainMain);
        String rootPath = arg0.getServletContext().getRealPath("/");
        logger.info("----------------rootPath=" + rootPath);
        //		Globals.contextRoot = rootPath ;
        
    }

    private void registerUrls(ServletContext ctx) {
    	try {
    		Field[] fields = OpiConstants.class.getFields();
        	for(Field field : fields) {
        		String name = field.getName();
        		Class<?> type = field.getType();
				if (name.startsWith("url")
						&& (type.equals(DyString.class) 
								|| type.equals(String.class))) {
					ctx.setAttribute(name, field.get(null));
				}
        	}
		} catch (Exception e) {
			e.printStackTrace();
		}
    }
    
    private static void test() {
    	try {
    		Field[] fields = OpiConstants.class.getFields();
        	for(Field field : fields) {
        		String name = field.getName();
        		//Class<?> type = field.getType();
        		if (name.startsWith("url")) {
        			System.out.println(name + "=" + field.get(null));
        		}
        	}
		} catch (Exception e) {
			e.printStackTrace();
		}
    	
    }
    
    public void contextDestroyed(ServletContextEvent arg0) {
    	
    }

    /*private String getDomain(String domain) {
        try {
            if (domain != null && domain.trim().length() > 0) {
                return "url".concat(domain.substring(0, 1).toUpperCase()).concat(
                        domain.substring(1));
            }
        } catch (Exception e) {
            // TODO: handle exception
        }
        return null;
    }*/

    public static void main(String[] args) {
		test();
	}
    
}

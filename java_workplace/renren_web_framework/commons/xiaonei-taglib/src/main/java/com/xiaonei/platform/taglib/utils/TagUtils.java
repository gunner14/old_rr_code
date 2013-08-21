package com.xiaonei.platform.taglib.utils;

import java.io.IOException;

import javax.servlet.jsp.JspWriter;
import javax.servlet.jsp.PageContext;

import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 
 */

public class TagUtils{
	
	public static void out(PageContext pageContext ,String text){
		JspWriter w = pageContext.getOut();
		try {
			w.print(text);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	/**
	 * 将原始的templet中的类似 ${applicationScope.urlMain} 都替换成相应的实际url，如http://xiaonei.com
	 * @param originalTemplet
	 * @return
	 */
	public static String replaceTempletURL(String originalTemplet){
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlClass}"), Constants.urlClass);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlTeam}"), Constants.urlTeam);
		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlMain}"), OpiConstants.urlMain.toString());
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlSearch}"), Constants.urlSearch);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlUpload}"), Constants.urlUpload);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlGroup}"), Constants.urlGroup);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlEvent}"), Constants.urlEvent);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlCourse}"), Constants.urlCourse);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlBlog}"), Constants.urlBlog);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlPhoto}"), Constants.urlPhoto);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlMsg}"), Constants.urlMsg);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlHome}"), Constants.urlHome);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlReg}"), Constants.urlReg);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlBrowse}"), Constants.urlBrowse);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlSchool}"), Constants.urlSchool);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlImg}"), Constants.urlImg);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlHead}"), Constants.urlHead);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlTxt5q}"), Constants.urlTxt5q);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlStatic}"), Constants.urlStatic);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlPhotoAlbum}"), Constants.urlPhotoAlbum);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlPhotoGroup}"), Constants.urlPhotoGroup);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlPhoto2Album}"), Constants.urlPhoto2Album);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlPhoto2Group}"), Constants.urlPhoto2Group);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlGuide}"), Constants.urlGuide);
//		originalTemplet = originalTemplet.replaceAll(getUrlRegEx("${applicationScope.urlAdmin}"), Constants.urlAdmin);
		return originalTemplet;
	}
	private static String getUrlRegEx(String originalUrl){
		//${applicationScope.urlMain}
		originalUrl = originalUrl.replace("{", "\\{");
		originalUrl = originalUrl.replace("$", "\\$");
//		System.out.println("regex 1:"+originalUrl);
		return originalUrl;
	}
	public static void main(String[] art){
		String rel = replaceTempletURL("abc${applicationScope.urlMain}edf");
		System.out.println("============rel="+rel);
		
//		String a  = "a$b";
//		String b = a.replaceAll("\\$", "@");
//		System.out.println("b="+b);
	}
}

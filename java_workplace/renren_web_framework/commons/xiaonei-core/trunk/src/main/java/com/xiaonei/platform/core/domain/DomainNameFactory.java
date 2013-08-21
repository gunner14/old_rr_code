package com.xiaonei.platform.core.domain;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.service.util.descriptor.SvcDescriptorAdapter;



public class DomainNameFactory {
	
	public static final String urlWww="www" ;
	public static final String urlLogin= "login" ;
	public static final String urlHome="home" ;
	public static final String urlMain="main" ;
	public static final String urlHead="head" ;
	public static final String urlTinyHead="tinyHead" ;
	public static final String urlClass = "class";
	public static final String urlTeam = "team" ; 
	public static final String urlGroup ="group";
	public static final String urlEvent = "event";
	public static final String urlCourse = "course";
	public static final String urlBlog = "blog";
	public static final String urlPhoto = "photo";
	public static final String urlMsg = "msg";
	public static final String urlUpload = "upload";
	public static final String urlSchool = "school";
	public static final String urlImg = "img";
	public static final String urlBrowse = "browse";
	public static final String urlAdmin = "admin";
	public static final String urlSupport = "support";
	public static final String urlStatic = "static" ;
	public static final String urlSource = "source" ;
    public static final String urlReg="reg" ;
    public static final String urlGuide = "guide" ;
    public static final String urlApp="app" ;
	public static final String urlPhotoAlbum = "photoAlbum";
    public static final String urlPhotoGroup = "photoGroup";
    public static final String urlPhoto2Album ="photo2Album";
    public static final String urlPhoto2Group = "photo2Group";
    
    public static void main(String[] args) throws Exception{
    	//DomainDAO.getInstance().saveDomainName("www", "http://www.xiaonei.com");
    	//DomainDAO.getInstance().saveDomainName("photoAlbum", "http://photos.album.5q.com");
    	//DomainDAO.getInstance().saveDomainName("photoGroup", "http://photos.group.5q.com");
    	//DomainDAO.getInstance().saveDomainName("photo2Album", "http://photos2.album.5q.com");
    	//DomainDAO.getInstance().saveDomainName("photo2Group", "http://photos2.group.5q.com");
    	//String main=DomainNameFactory.getInstance().getDomainName(urlMain);
    	//System.out.println(main);
	}
	
	private static DomainNameFactory instance = null;
	private static Map<String,String> domains=new HashMap<String,String>();
	public static DomainNameFactory getInstance(){
		return (instance==null)?new DomainNameFactory():instance;
	}
	
	private DomainNameFactory(){
		initDomainName();
		SvcDescriptorAdapter.getInstance().subscribe(DomainNameListener.class.toString(), new DomainNameListener());
	}
	
	public void initDomainName(){
		List<DomainName> domainList=null;
		try {
			domainList=(List<DomainName>)DomainDAO.getInstance().getDomainName();
			if(domainList.size()>0){
				domains.clear();
				for(DomainName dn:domainList){
					System.out.println("domainname:"+dn.getDomainName()+"====url:"+dn.getDomainUrl());
					domains.put(dn.getDomainName(), dn.getDomainUrl());
				}
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	public String getDomainUrlByName(String key){
		if(key==null){
			return null;
		}
		if(domains==null){
			initDomainName();
		}
		return domains.get(key);
	}
	//开心上无此方法
	public Map<String,String> getDomainMap(){
		return domains;
	}
}

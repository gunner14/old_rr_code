<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="java.net.URLEncoder"%>
<%

	response.setContentType("application/x-download");//设置为下载application/x-download
	String filedownload = "/rrw.url";//即将下载的文件的相对路径
	String filedisplay = "人人网.url";//下载文件时显示的文件保存名称
	String agent = request.getHeader("USER-AGENT");
	//System.out.println("0 "+agent);
	String finalFileName=filedisplay;
	try{
	    finalFileName=processFileName(filedisplay, agent);
	}
	catch(Exception e){	   
	    finalFileName=URLEncoder.encode(filedisplay, "UTF-8");
	}
	
	if(null != agent && (-1 != agent.indexOf("Safari") && -1 == agent.indexOf("Chrome"))){ //Safari
	    response.addHeader("Content-Disposition","attachment;filename="+new String(finalFileName.getBytes("GBK"),"ISO-8859-1"));
	}
	else{
		response.addHeader("Content-Disposition", "attachment;filename="+ finalFileName);
	}
	
	try{
		RequestDispatcher dis = application.getRequestDispatcher(filedownload);
		if (dis != null){
			dis.forward(request, response);
		}
		response.flushBuffer();
	}
	catch (Exception e){
		e.printStackTrace();
	}
	finally {
	}
%>
<%!
	String processFileName(String fileName, String agent) throws Exception {
		String codedfilename = null;
		if (null != agent && (-1 != agent.indexOf("MSIE"))) {
			String prefix = fileName.lastIndexOf(".") != -1 ? fileName.substring(0, fileName.lastIndexOf(".")) : fileName;
			String extension = fileName.lastIndexOf(".") != -1 ? fileName.substring(fileName.lastIndexOf(".")) : "";
			String name = java.net.URLEncoder.encode(fileName, "UTF8");
			if (name.lastIndexOf("%0A") != -1) {
				name = name.substring(0, name.length() - 3);
			}
			int limit = 150 - extension.length();
			if (name.length() > limit) {
				name = java.net.URLEncoder.encode(prefix.substring(0, Math.min(prefix.length(), limit / 9)), "UTF-8");
				if (name.lastIndexOf("%0A") != -1) {
					name = name.substring(0, name.length() - 3);
				}
			}
			codedfilename = name ;//+ extension;
			////System.out.println("7 "+codedfilename);
		} 
		else if (null != agent && -1 != agent.indexOf("Chrome")) { //Safari
			codedfilename = "=?UTF-8?B?" + (new String(org.apache.commons.codec.binary.Base64.encodeBase64(fileName.getBytes("UTF-8")))) + "?=";
            //System.out.println("8 "+codedfilename);
        } 
		else if (null != agent && -1 != agent.indexOf("Safari")) { //Safari
            codedfilename = fileName;//(new String(org.apache.commons.codec.binary.Base64.encodeBase64(fileName.getBytes("UTF-8"))));
            //System.out.println("5 "+codedfilename);
        } 
		else if (null != agent && -1 != agent.indexOf("Mozilla") && -1 == agent.indexOf("Safari")) { //Firefox 可以改下判断
			codedfilename = "=?UTF-8?B?" + (new String(org.apache.commons.codec.binary.Base64.encodeBase64(fileName.getBytes("UTF-8")))) + "?=";
			//System.out.println("5 "+codedfilename);
		} 
		else {
			codedfilename = fileName;
			//System.out.println("6 "+codedfilename);
		}
		return codedfilename;
	}
%>
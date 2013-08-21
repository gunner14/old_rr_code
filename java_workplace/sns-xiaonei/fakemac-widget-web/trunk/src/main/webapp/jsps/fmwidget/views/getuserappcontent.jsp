
<%@page import="org.json.JSONObject"%>
<%@page import="org.json.JSONArray"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel"%><%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel"%>
<%@page import="java.util.List"%><%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>${curm.content}<% if(request.getAttribute("curm")==null){ 
    List<FakemacWidgetRecordModel> list = (List<FakemacWidgetRecordModel>)request.getAttribute("records");
    JSONObject rjo = new JSONObject();
    JSONArray ja = new JSONArray();
    StringBuilder sb = new StringBuilder();
    
    for(FakemacWidgetRecordModel m : list){
    	if("".equals(m.getContent())){continue;}
        JSONObject jo = new JSONObject();
        /*jo.put("id",m.getId());
        jo.put("appid",m.getAppId());
        jo.put("userid",m.getUserId());
        jo.put("config",m.getConfig());*/
        jo.put("position",m.getPosition());
        jo.put("content",m.getContent());
        System.out.println(m+" - "+jo.toString());
        ja.put(jo);
        sb.append(","+m.getContent()+"");
    }
    
    rjo.put("datas",ja);
    String ctc = sb.toString();
    if(ctc == null || ctc.equals("")){
        
    }
    else {
        ctc = ctc.substring(1);
    }
    out.println("["+ctc+"]");
}%>

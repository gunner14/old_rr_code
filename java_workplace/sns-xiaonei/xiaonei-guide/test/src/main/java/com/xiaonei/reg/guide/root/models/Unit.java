package com.xiaonei.reg.guide.root.models;

import com.xiaonei.reg.guide.root.configs.StepStatusDefinition;
import com.xiaonei.reg.guide.util.GuideLogger;



public class Unit {
	private String stepflagname;
	private String name;
	private String url;
	private String remark;
	private String id;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public int getBinaryFlag(){
		return StepStatusDefinition.getBinaryFlag(stepflagname);
	}
	public String getStepflagname() {
		return stepflagname;
	}
	public void setStepflagname(String stepname) {
		this.stepflagname = stepname;
	}
	public Unit(String id,String stepname, String name, String url, String remark) {
		super();
		this.id = id;
		this.stepflagname = stepname;
		this.name = name;
		this.url = url;
		this.remark = remark;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getUrl() {
		return url;
	}
	public void setUrl(String url) {
		this.url = url;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
	public void printAll(){
		GuideLogger.printLog(" |uid:"+id+" uname:"+name+" url:"+url+"|");
	}
	public String getHumanReadableInfo(){
		return "uid:"+id+" uname:"+name+" url:"+url+"|"+"("+remark+")";
	}

}

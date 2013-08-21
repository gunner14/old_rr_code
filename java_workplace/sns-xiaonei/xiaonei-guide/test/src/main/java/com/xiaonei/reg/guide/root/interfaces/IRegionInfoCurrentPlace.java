package com.xiaonei.reg.guide.root.interfaces;

public interface IRegionInfoCurrentPlace extends IXProfilable{
	public String getCurrentprovince();
	public void setCurrentprovince(String currentprovince);
	public String getCurrentcityname();
	public void setCurrentcityname(String currentcityname);
	public int getCurrentcitycode();
	public void setCurrentcitycode(int currentcitycode);
	
	public int getCurrentregionid();
	public void setCurrentregionid(int currentregionid);
}

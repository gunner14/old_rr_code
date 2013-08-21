package com.xiaonei.reg.guide.flows.xfive.logics;

import com.xiaonei.reg.guide.root.interfaces.IInfoElementaryschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoJuniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoSeniorhighschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoTechschool;
import com.xiaonei.reg.guide.root.interfaces.IInfoUniversity;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoCurrentPlace;
import com.xiaonei.reg.guide.root.interfaces.IRegionInfoHomePlace;
import com.xiaonei.reg.guide.root.interfaces.IWorkInfoCompany;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;

public class GuideXFiveUserInfoCheckLogic {
	private static GuideXFiveUserInfoCheckLogic instance = new GuideXFiveUserInfoCheckLogic();
	public static final String RET_INFO_OK = "OK";
	public static final String RET_INFO_ERROR = " info with err: ";
	public static final String RET_INFO_NULL = " info is null ";
    public static GuideXFiveUserInfoCheckLogic getInstance() {
        return instance;
    }
   
    public String isAUniversity(IInfoUniversity iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( iinfo.getUniversitycode() == 0
    			|| MyStringUtils.isNullOrEmpty(iinfo.getUniversityname())
    			|| MyStringUtils.contains(iinfo.getUniversityname(), "选择大学")
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	
    	return ret;
    }
    
    public String isFullUniversity(IInfoUniversity iinfo){
    	String ret = isAUniversity(iinfo);
    	if(ret == RET_INFO_OK){
    		if(MyStringUtils.isNullOrEmpty(iinfo.getUniversitydepartment())
    			|| iinfo.getUniversityyear() == 0
    			|| MyStringUtils.isNullOrEmpty(iinfo.getCoursetype())
    		){
    			ret = RET_INFO_ERROR+iinfo.toString();
    		}
    	}
    	
    	return ret;
    }
    
    public String isASeniorhighschool(IInfoSeniorhighschool iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( iinfo.getHighschoolcode() == 0
    			|| MyStringUtils.isNullOrEmpty(iinfo.getHighschoolname())
    			|| MyStringUtils.contains(iinfo.getHighschoolname(), "选择高中")
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	
    	return ret;
    }

    public String isFullSenirohighschool(IInfoSeniorhighschool iinfo){
    	String ret = isASeniorhighschool(iinfo);
    	if(ret == RET_INFO_OK){
    		if(iinfo.getHighschoolyear() == 0
				|| MyStringUtils.isNullOrEmpty(iinfo.getHighschoolclass1())
				|| MyStringUtils.isNullOrEmpty(iinfo.getHighschoolclass2())
    			|| MyStringUtils.isNullOrEmpty(iinfo.getHighschoolclass3())
    		){
    			ret = RET_INFO_ERROR+iinfo.toString();
    		}
    	}
    	
    	return ret;
    }
    
    public String isAJuniorhighschool(IInfoJuniorhighschool iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( iinfo.getJuniorhighschoolcode() == 0
    			|| MyStringUtils.isNullOrEmpty(iinfo.getJuniorhighschoolname())
    			|| MyStringUtils.contains(iinfo.getJuniorhighschoolname(), "选择初中")
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	return ret;
    }
    
    public String isFullJuniorhighschool(IInfoJuniorhighschool iinfo){
    	String ret = isAJuniorhighschool(iinfo);
    	if(ret == RET_INFO_OK){
    		if(iinfo.getJuniorhighschoolyear() == 0
    		){
    			ret = RET_INFO_ERROR+iinfo.toString();
    		}
    	}
    	
    	return ret;
    }
    public String isATechschool(IInfoTechschool iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( iinfo.getTechschoolcode() == 0
    			|| MyStringUtils.isNullOrEmpty(iinfo.getTechschoolname())
    			|| MyStringUtils.contains(iinfo.getTechschoolname(), "选择中专")
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	
    	return ret;
    }
    
    public String isFullTechschool(IInfoTechschool iinfo){
    	String ret = isATechschool(iinfo);
    	if(ret == RET_INFO_OK){
    		if(iinfo.getTechschoolyear() == 0
    		){
    			ret = RET_INFO_ERROR+iinfo.toString();
    		}
    	}
    	
    	return ret;
    }
    public String isAElementaryschool(IInfoElementaryschool iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( iinfo.getElementaryschoolcode() == -1
    			|| MyStringUtils.isNullOrEmpty(iinfo.getElementaryschoolname())
    			|| MyStringUtils.contains(iinfo.getElementaryschoolname(), "填写小学")
    			
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	return ret;
    }
    
    public String isFullElementaryschool(IInfoElementaryschool iinfo){
    	String ret = isAElementaryschool(iinfo);
    	if(ret == RET_INFO_OK){
    		if(iinfo.getElementaryschoolyear() == 0
    		){
    			ret = RET_INFO_ERROR+iinfo.toString();
    		}
    	}
    	
    	return ret;
    }
    public String isACurrentPlaceInfo(IRegionInfoCurrentPlace iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( iinfo.getCurrentcitycode() == 0
    			|| MyStringUtils.isNullOrEmpty(iinfo.getCurrentprovince())
    			|| MyStringUtils.isNullOrEmpty(iinfo.getCurrentcityname())
    			|| MyStringUtils.equals(iinfo.getCurrentcityname(), "城市")
    			|| MyStringUtils.contains(iinfo.getCurrentprovince(), "省份")
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	
    	return ret;
    }
    public String isAHomePlaceInfo(IRegionInfoHomePlace iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( iinfo.getHomecitycode() == 0
    			|| MyStringUtils.isNullOrEmpty(iinfo.getHomeprovince())
    			|| MyStringUtils.isNullOrEmpty(iinfo.getHomecityname())
    			|| MyStringUtils.equals(iinfo.getHomecityname(), "城市")
    			|| MyStringUtils.contains(iinfo.getHomeprovince(), "省份")
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	
    	return ret;
    }
    public String isAWorkInfo(IWorkInfoCompany iinfo){
    	String ret = RET_INFO_OK;
    	if(iinfo == null){
    		ret = RET_INFO_NULL;
    	}
    	else if( MyStringUtils.isNullOrEmpty(iinfo.getCompanyname())
    			|| MyStringUtils.contains(iinfo.getCompanyname(), "填写单位")
    	){
    		ret = RET_INFO_ERROR+iinfo.toString();
    	}
    	else{
    		ret = RET_INFO_OK;
    	}
    	
    	return ret;
    }
    
    
    
}

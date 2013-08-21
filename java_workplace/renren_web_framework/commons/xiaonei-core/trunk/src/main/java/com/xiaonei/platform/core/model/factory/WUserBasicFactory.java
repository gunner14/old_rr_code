package com.xiaonei.platform.core.model.factory;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.xce.userbasic.UserBasicFactory;

public class WUserBasicFactory implements UserBasicFactory<UserBasic> {
	       public UserBasic create(){
	    	   return new UserBasic() ; 
	       }
	}


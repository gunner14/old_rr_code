package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.xce.userdescview.UserDescViewFactory;

public class WUserDescFactory implements UserDescViewFactory<UserDesc>{
	 public UserDesc create(){
  	   return new UserDesc() ; 
     }
}

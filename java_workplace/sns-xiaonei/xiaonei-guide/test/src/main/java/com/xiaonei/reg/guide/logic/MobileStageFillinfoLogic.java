/**
 *
 */
package com.xiaonei.reg.guide.logic;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.username.UserNameInfo;

/**
 * MobileStageFillinfoLogic
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午04:11:46
 */
public class MobileStageFillinfoLogic {

    /**
     * 保存stage
     *
     * @param stage
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午04:39:01
     */
    public static void saveStage(User user, int stage) {
    	FillInfoLogic.getInstance().saveStage(user, stage);
    }

    /**
     * 保存name
     *
     * @param name
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午04:40:07
     */
    public static void saveName(User user, String name) {
    	GuideLogger.printLog(" host:"+user.getId() + " savename:"+name+" old:"+user.getName());
    	if("".equals("")){
    		String curName = user.getName();
    		if(curName == null){
    			curName = "";
    		}
    		curName = curName.trim();
    		if(name == null){
    			name = "";
    		}
    		
    		name = name.trim();
    		
    		if(curName.startsWith("新用户") || "".equals(curName)){
    			if(!name.startsWith("新用户") && !"".equals(name)){
    				 try {
    			        	UserNameInfo userNameInfo = new UserNameInfo();
    			        	userNameInfo.setName(name);
    			        	WUserAdapter.getInstance().getNameAdapter().setUserName(user.getId(), userNameInfo);
    			        } catch (Exception e) {
    			            System.err.println("Exception User id: " + user.getId());
    			            e.printStackTrace();
    			        }
    			        try {
    			        	UserName un = SnsAdapterFactory.getUserNameAdapter().getUserName(user.getId());
    			        	un.setName(name);
    			        	SnsAdapterFactory.getUserNameAdapter().setUserName(user.getId(), un);
    			        	//2010-01-20 拆分UserBasic
    			           /* UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(user.getId());
    			            ub.setName(name);
    			            UserAdapter.setBasic(ub);*/
    			        } catch (Exception e) {
    			            System.err.println("Exception User id: " + user.getId());
    			            e.printStackTrace();
    					}
    			}
    			else{
    				GuideLogger.printLog(" host:"+user.getId() + " will to name:"+name, GuideLogger.WARN);
    			}
    		}
    		else{
    			GuideLogger.printLog(" host:"+user.getId() + " refill name:"+name, GuideLogger.WARN);
    		}
    		GuideLogger.printLog(" host:"+user.getId() + "old:"+curName + " name:"+name);
    		return;
    	}
       
	}

}

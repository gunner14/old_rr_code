package com.xiaonei.reg.activity.msntab.logic;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveIDsPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.sns.platform.core.opt.ice.IUserCacheAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;


public class InviteForMSNTabLogic {
	
	static public class MSNUserInfo{
		public String uname;
		public String uhead;
		public String getUname() {
			return uname;
		}
		public void setUname(String uname) {
			this.uname = uname;
		}
		public String getUhead() {
			return uhead;
		}
		public void setUhead(String uhead) {
			this.uhead = uhead;
		}
	}
	private static final int MAX = 6;
	private static InviteForMSNTabLogic instance = new InviteForMSNTabLogic();

    public static InviteForMSNTabLogic getInstance() {
        return instance;
    }
    
    public List<MSNUserInfo> getRelations(User host){
    	List<MSNUserInfo> infolist = new ArrayList<MSNUserInfo>();
    	try {
    		//List<InviteInfo> ulist = new ArrayList<InviteInfo>();
    		List<InviteInfo> info_WhoInviteI = InviteDAO.getInstance().getInviteUsers(host.getAccount());
    		if(info_WhoInviteI != null){
    			for (InviteInfo inviteInfo : info_WhoInviteI) {
    				MSNUserInfo msninfo = new MSNUserInfo();
    				msninfo.uname = inviteInfo.getOwner_name();
    				msninfo.uhead = inviteInfo.getTinyurl();
				}
    		}
    		if(infolist.size() < MAX){
    			/*List<InviteInfo> info_IInviteWho = InviteDAO.getInstance().getInviteInfoByOwnerId(host.getId(), 1, 0, 10);
    			if(info_IInviteWho != null){
    				for (InviteInfo inviteInfo : info_IInviteWho) {
    					MSNUserInfo msninfo = new MSNUserInfo();
    					msninfo.name = inviteInfo.getAccountname();
					}
    			}*/
    		}
    		
    		
    		if(infolist.size() < MAX){
    			List<Integer> plist = GuideXFiveIDsPreviewLogic.getInstance().getProfileResults(MAX, host, 0);
    			if(plist == null){
    				GuideLogger.printLog(" host:"+host.getId());
    			}
    			else{
    				GuideLogger.printLog(" host:"+host.getId()+" plist:"+plist.size());
    				List<MSNUserInfo> infos = getUserInfo(plist);
    				GuideLogger.printLog(" host:"+host.getId()+" infos:"+infos.size());
    				infolist.addAll(infos);
    			}
    		}
    		
    		
		} 
    	catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch(Exception e){
			e.printStackTrace();
		}
    	
    	return infolist;
    }
    
    private static Map<Integer, WUserCache> getUserCacheMap(List<Integer> idlist){
		IUserCacheAdapter adapter = null;
		try {
			adapter = SnsAdapterFactory.getUserCacheAdapter();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Map<Integer, WUserCache> ucl = null;
		if(adapter != null){
			try {
				ucl = adapter.getUserCacheMap(idlist);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if(ucl == null){
			ucl = new HashMap<Integer, WUserCache>();
		}
		return ucl;
	}
    
    public static List<MSNUserInfo> getUserInfo(List<Integer> ids) {
    	List<MSNUserInfo> ulist = new ArrayList<MSNUserInfo>();
		try {
			Map<Integer, WUserCache> ucl = getUserCacheMap(ids);
			// 返回数据
			
			int i = 0;
			for (int curid : ids) {
				i++;
				WUserCache theuser = ucl.get(curid);
				if(theuser != null){
					MSNUserInfo info = new MSNUserInfo();
					info.uname = theuser.getName();
					info.uhead = theuser.getTiny_Url();
					ulist.add(info);
				}
				
				
			}
			
			for(;i < 5;i++){
				MSNUserInfo info = new MSNUserInfo();
				info.uname = names[4 - i];
				info.uhead = heads[4 - i];
				ulist.add(info);
			}
			
			
		} catch (Exception e) {
			GuideLogger.printLog(" e:"+e.toString());
			e.printStackTrace();
		}
		return ulist;
	}
    
	private static String [] heads={
		"http://hdn.xnimg.cn/photos/hdn601/20090202/18/15/tiny_CFOI_5440n198107.jpg",
		"http://hdn.xnimg.cn/photos/hdn321/20100208/1600/tiny_ERSA_52310o019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn321/20091215/1025/tiny_m5NT_16516g019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn421/20100302/2310/tiny_tkSg_162390j019116.jpg",
		"http://hdn.xnimg.cn/photos/hdn121/20100206/0135/tiny_CdHc_39567f019118.jpg"
	};
    private static String [] names={
    	"表妹",
    	"学姐",
    	"哥们",
    	"老师",
    	"同事"
    };
}

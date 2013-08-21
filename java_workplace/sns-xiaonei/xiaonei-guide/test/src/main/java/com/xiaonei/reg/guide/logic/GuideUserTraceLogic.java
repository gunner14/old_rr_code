package com.xiaonei.reg.guide.logic;

import java.util.HashSet;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;

public class GuideUserTraceLogic {
    private GuideUserTraceLogic() {
        super();
    }

    private static GuideUserTraceLogic _instance = new GuideUserTraceLogic();

    public static GuideUserTraceLogic getInstance() {
        return _instance;
    }
    
    @SuppressWarnings("unused")
	private void test(HttpServletRequest request, User host, String email, String actionIdStr, int stage, String s1, String s2, String s3, String s4, int inviter_id){
    	UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(), "01010101", host.getStage(), "", "", "1", "", 81120);
    }
    
    public void asyncWriteTrace(final HttpServletRequest request, final User host, final String actionid){
		int i = BirthdayLogic.getInstance().less17age(host)?0:1;
		GuideLogger.printLog("host:"+host.getId()+" - "+actionid+" - age:"+i);
		UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(), actionid, host.getStage(), "", "", String.valueOf(i), "", 0);
    	GuideLogger.printLog("host:"+host.getId()+" - "+actionid);
    }
    
    public static final String STEP001_ARRIVAL_OPENFLOW = "10101001";
    public static final String STEP001_CONFIRM_OPENFLOW = "10101002";
    
    public static final String STEP002_ARRIVAL_FILLINFO = "10102001";
    public static final String STEP002_CONFIRM_FILLINFO = "10102002";
    
    public static final String STEP003_ARRIVAL_PREVIEW = "10103001";
    public static final String STEP003_CONFIRM_PREVIEW = "10103002";
    public static final String STEP003_SKIPPED_PREVIEW = "10103003";
    
    public static final String STEP004_ARRIVAL_SEECERTIFICATE = "10104001";
    public static final String STEP004_CONFIRM_SEECERTIFICATE = "10104002";
    
    public static final String STEP005_ARRIVAL_UPLOAD = "10105001";
    public static final String STEP005_CONFIRM_UPLOAD = "10105002";
    
    //==============================================================================================================
    
    public static final String COOK_STEP001_ARRIVAL_OPENFLOW = "10201001";
    public static final String COOK_STEP001_CONFIRM_OPENFLOW = "10201002";
    
    public static final String COOK_STEP002_ARRIVAL_FILLINFO = "10202001";
    public static final String COOK_STEP002_CONFIRM_FILLINFO = "10202002";
    
    public static final String COOK_STEP003_ARRIVAL_PREVIEW = "10203001";
    public static final String COOK_STEP003_CONFIRM_PREVIEW = "10203002";
    public static final String COOK_STEP003_SKIPPED_PREVIEW = "10203003";
    
    public static final String COOK_STEP004_ARRIVAL_UPLOAD = "10204001";
    public static final String COOK_STEP004_CLICKED_UPLOAD = "10204002";
    public static final String COOK_STEP004_SKIPPED_UPLOAD = "10204003";
    public static final String COOK_STEP004_WELLDON_UPLOAD = "10204004";
    
  //==============================================================================================================
    
    public static final String NETBAR_STEP001_ARRIVAL_OPENFLOW = "20301001";
    public static final String NETBAR_STEP001_CONFIRM_OPENFLOW = "20301002";
    
    //2009-11-16 11:46:27 改的。因为网吧只剩一步了……以后要解开的话，要注意id！是和正常引导冲突的！
    /*
    public static final String NETBAR_STEP002_ARRIVAL_PREVIEW = "20102001";
    public static final String NETBAR_STEP002_CONFIRM_PREVIEW = "20102002";
    public static final String NETBAR_STEP002_SKIPPED_PREVIEW = "20102008";
    
    public static final String NETBAR_STEP004_ARRIVAL_UPLOAD = "20106001";
    public static final String NETBAR_STEP004_UPLOADED_UPLOAD = "20106002";
    public static final String NETBAR_STEP004_CAMERA_UPLOAD = "20106003";
    public static final String NETBAR_STEP004_SAVED_UPLOAD = "20106004";
    public static final String NETBAR_STEP004_SKIPPED_UPLOAD = "20106008";
    */
    
    //==============================================================================================================
    public static final String BYSTAGE_STEP001_ARRIVAL_FILLINFO = "20101001";
    public static final String BYSTAGE_STEP001_CONFIRM_FILLINFO = "20101002";
    public static final String BYSTAGE_STEP001_CONFIRM_ZX_FILLINFO = "20101004";
    public static final String BYSTAGE_STEP001_CONFIRM_DX_FILLINFO = "20101005";
    public static final String BYSTAGE_STEP001_CONFIRM_BL_FILLINFO = "20101006";
    public static final String BYSTAGE_STEP001_CONFIRM_QT_FILLINFO = "20101007";
    
    public static final String BYSTAGE_STEP002_ARRIVAL_PREVIEW = "20102001";
    public static final String BYSTAGE_STEP002_CONFIRM_PREVIEW = "20102002";
    public static final String BYSTAGE_STEP002_SKIPPED_PREVIEW = "20102008";
    
    
    public static final String BYSTAGE_STEP003_ARRIVAL_IMPORT = "20103001";
    public static final String BYSTAGE_STEP003_COMFIRM_MSN_IMPORT = "20103002";
    public static final String BYSTAGE_STEP003_COMFIRM_MAIL_IMPORT = "20103003";
    public static final String BYSTAGE_STEP003_SKIPPED_IMPORT = "20103008"; 
    
    public static final String BYSTAGE_STEP004_ARRIVAL_PORTRAIT = "20106001";
    public static final String BYSTAGE_STEP004_UPLOAD_PORTRAIT = "20106002";
    public static final String BYSTAGE_STEP004_CAMARA_PORTRAIT = "20106003";
    public static final String BYSTAGE_STEP004_SAVE_PORTRAIT = "20106007";
    public static final String BYSTAGE_STEP004_SKIPPED_PORTRAIT = "20106008";
    public static final String BYSTAGE_STEP004_SAVE_PORTRAIT_NOFLASH = "20106009";
    public static final String BYSTAGE_STEP004_SKIPPED_PORTRAIT_NOFLASH = "20106010";
    
    
    private static Set<String> netbarset = new HashSet<String>();
    private static Set<String> bystageset = new HashSet<String>();
    static {
    	netbarset.add(NETBAR_STEP001_ARRIVAL_OPENFLOW);
    	netbarset.add(NETBAR_STEP001_CONFIRM_OPENFLOW);
    	
    	/*
    	netbarset.add(NETBAR_STEP002_ARRIVAL_PREVIEW);
    	netbarset.add(NETBAR_STEP002_CONFIRM_PREVIEW);
    	netbarset.add(NETBAR_STEP002_SKIPPED_PREVIEW);
    	
    	netbarset.add(NETBAR_STEP004_ARRIVAL_UPLOAD);
    	netbarset.add(NETBAR_STEP004_UPLOADED_UPLOAD);
    	netbarset.add(NETBAR_STEP004_CAMERA_UPLOAD);
    	netbarset.add(NETBAR_STEP004_SAVED_UPLOAD);
    	netbarset.add(NETBAR_STEP004_SKIPPED_UPLOAD);
    	*/
    	
    	bystageset.add(BYSTAGE_STEP001_ARRIVAL_FILLINFO);
    	bystageset.add(BYSTAGE_STEP001_CONFIRM_FILLINFO);
    	bystageset.add(BYSTAGE_STEP001_CONFIRM_ZX_FILLINFO);
    	bystageset.add(BYSTAGE_STEP001_CONFIRM_DX_FILLINFO);
    	bystageset.add(BYSTAGE_STEP001_CONFIRM_BL_FILLINFO);
    	bystageset.add(BYSTAGE_STEP001_CONFIRM_QT_FILLINFO);
    	
    	bystageset.add(BYSTAGE_STEP002_ARRIVAL_PREVIEW);
    	bystageset.add(BYSTAGE_STEP002_CONFIRM_PREVIEW);
    	bystageset.add(BYSTAGE_STEP002_SKIPPED_PREVIEW);
    	
    	bystageset.add(BYSTAGE_STEP003_ARRIVAL_IMPORT);
    	bystageset.add(BYSTAGE_STEP003_COMFIRM_MSN_IMPORT);
    	bystageset.add(BYSTAGE_STEP003_COMFIRM_MAIL_IMPORT);
    	bystageset.add(BYSTAGE_STEP003_SKIPPED_IMPORT);
    	
    	bystageset.add(BYSTAGE_STEP004_ARRIVAL_PORTRAIT);
    	bystageset.add(BYSTAGE_STEP004_UPLOAD_PORTRAIT);
    	bystageset.add(BYSTAGE_STEP004_CAMARA_PORTRAIT);
    	bystageset.add(BYSTAGE_STEP004_SKIPPED_PORTRAIT);
    	bystageset.add(BYSTAGE_STEP004_SAVE_PORTRAIT);
    	bystageset.add(BYSTAGE_STEP004_SAVE_PORTRAIT_NOFLASH);
    	bystageset.add(BYSTAGE_STEP004_SKIPPED_PORTRAIT_NOFLASH);
    	
    }
    
    public boolean checkNetbarActionId(String id){
    	return netbarset.contains(id);
    }

    public boolean checkBystargeActionId(String id){
    	return bystageset.contains(id);
    }
    
    
}

package com.renren.sns.lead.dataproviders.util;

import org.apache.log4j.Logger;

public class StatLogger {
	private static final Logger logger = Logger.getLogger("xnlog.guide");
	//到达 _ARV
	//行为 _ACT
	//展现 _RST
	public static final String S_RST_FINDFRIEND_PV_ = "S_RST_FINDFRIEND_PV_";
	public static final String S_RST_FINDFRIEND_MF_ = "S_RST_FINDFRIEND_MF_";
	
	public static final String S_ACT_ADDEDFRIEN_FI_ = "S_ACT_ADDEDFRIEN_FI_";
	public static final String S_ACT_NOADDFRIEN_FI_ = "S_ACT_NOADDFRIEN_FI_";
	
	
	public static final String S_ARV_ENTERINTOO_FI_ = "S_ARV_ENTERINTOO_FI_";
	public static final String S_ARV_ENTERINTOO_PV_ = "S_ARV_ENTERINTOO_PV_";
	public static final String S_ARV_ENTERINTOO_MF_ = "S_ARV_ENTERINTOO_MF_";
	public static final String S_ARV_ENTERINTOO_IM_ = "S_ARV_ENTERINTOO_IM_";
	public static final String S_ARV_ENTERINTOO_PO_ = "S_ARV_ENTERINTOO_PO_";
	public static final String S_ARV_ENTERINTOO_LN_ = "S_ARV_ENTERINTOO_LN_";
	
	
	public static final String TYPE_AAF = "AAF";
	public static final String TYPE_AAJ = "AAJ";
	public static final String TYPE_AAO = "AAO";
	public static final String TYPE_AAP = "AAP";
	public static final String TYPE_AAQ = "AAQ";
	public static final String TYPE_AAR = "AAR";
	
	@SuppressWarnings("unused")
	private static void info(int hostid, String message){
		logger.info(""+hostid+" "+message);
	}
	
	public static void info(int hostid, String message, String prefix,String usertype){
		logger.info(""+hostid+" "+prefix+usertype+" "+message);
	}
}

package com.xiaonei.reg.guide.logic;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.action.a090729.A090729Util;
import com.xiaonei.reg.guide.dao.StepStatusDAO;
import com.xiaonei.reg.guide.flows.xfive.controllers.ArgsController;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.reg.register.model.RegFromSource;
import com.xiaonei.reg.usertrace.dao.GuideRecommendDAO;
import com.xiaonei.xce.log.TimeCost;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * StepStatusLogic
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午05:22:33
 */
public class StepStatusLogic{
	
	
	

	/** 完善资料 2 */
    public static final int STEP_FILLINFO = Integer.valueOf("10", 2); // 完善资料

    /** 上传头像 4 */
    public static final int STEP_UPLOAD_HEAD = Integer.valueOf("100", 2); // 上传头像

    /** 导入MSN 8 */
    public static final int STEP_IMPORT_MSN = Integer.valueOf("1000", 2); // 导入MSN

    /** 正式开始页 16 */
    public static final int STEP_PREVIEW = Integer.valueOf("10000", 2); // 正式开始页

    /** guide页 32 */
    public static final int STEP_GUIDE = Integer.valueOf("100000", 2);

    /** 手机注册，fillinfo 64 */
    public static final int STEP_FILLINFO_MOBILE = Integer.valueOf("1000000", 2);

    /** 手机注册，邮箱验证页 128 */
    public static final int STEP_VERIFY_MOBILE = Integer.valueOf("10000000", 2);

    /** 256 */
    //inshion 2009-9-4
    //public static final int STEP_FILLINFO_STAGE = Integer.valueOf("100000000", 2);
    //2009-11-21 12:56:03
    public static final int STEP_LANDING_PAGE = Integer.valueOf("100000000", 2); //256

    /** 引导完成标志（不含guide页） 4096*/
    public static final int STEP_COMPLETE_GUIDE = Integer.valueOf("1000000000000", 2);

    /** 4399完善资料 8192 */
    public static final int STEP_4399_FILLINFO = Integer.valueOf("10000000000000", 2);

    /** 手机注册,fillinfo 完善基本资料 16384 */
    public static final int STEP_FILLINFO_STAGE_MOBILE = Integer.valueOf("100000000000000", 2);

    /** 狗等APP 32768*/
    public static final int STEP_APP = Integer.valueOf("1000000000000000", 2);

    /** FLASH流程入口 65536 */
    public static final int STEP_FLASH_APP_OPEN = Integer.valueOf("10000000000000000", 2);
    
    /** 流程推荐入口 131072 */
	public static final int STEP_FlOW_RECOMMEND = Integer.valueOf("100000000000000000", 2);//131072

	private static final String MEM_CACHE_GUIDE_STATUS_ID = "mem_cache_guide_status_";
	
	
	 private static void insertNewflowLogRecord(final int userid,
	    		final int stage,final String fromSource,final int step,final String pageid ) {
	        Thread letter = new Thread() {
	            @Override
	            public void run() {
	                try {
	                	GuideRecommendDAO.getInstance().insert_newflow_log_record(userid, 
	                			stage,fromSource, step,pageid);
	                } catch (Exception e) {
	                	e.printStackTrace();
	                }
	            }
	        };
	        RunnableTaskExecutor.getInstance().runTask(letter);
	    }
	 
	 private static void insertGuideLogRecord(final int userid,
	    		final int stage,final String fromSource,final int step) {
	        Thread letter = new Thread() {
	            @Override
	            public void run() {
	                try {
	                	GuideRecommendDAO.getInstance().insert_guide_log_record(userid, 
	                			stage,
	                			fromSource, step);
	                } catch (Exception e) {
	                	e.printStackTrace();
	                }
	            }
	        };
	        RunnableTaskExecutor.getInstance().runTask(letter);
	    }

	/**
	 * 更新状态值
	 * 
	 * @param userId
	 * @param step
	 * @return 更新是否成功
	 */
	public static boolean updateStepStatus(final int userId, final int step) {
//		StackTraceElement stacke;
//		stacke = Thread.currentThread().getStackTrace()[3];
//		MyLogger.writeLine("[tr]StepStatusLogic::"+stacke.getClassName() + "." + stacke.getMethodName());
//		stacke = Thread.currentThread().getStackTrace()[2];
//		MyLogger.writeLine("[tr]StepStatusLogic::"+stacke.getClassName() + "." + stacke.getMethodName());
//		stacke = Thread.currentThread().getStackTrace()[1];
//		MyLogger.writeLine("[tr]StepStatusLogic::"+stacke.getClassName() + "." + stacke.getMethodName());
//		stacke = Thread.currentThread().getStackTrace()[0];
//		MyLogger.writeLine("[tr]StepStatusLogic::"+stacke.getClassName() + "." + stacke.getMethodName());
		boolean isOK = true;
		if("".equals("")){
			try {
				MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_user_space);
				String key = MEM_CACHE_GUIDE_STATUS_ID + userId;
				GuideLogger.printLog("host:" + userId + " - " + key);
				mem.set(key, Integer.valueOf(step | StepStatusLogic.getStepStatus(userId)),	(int) MemCacheKeys.minute * 30);
			} catch (SQLException e) {
				GuideLogger.printLog("save err host:" + userId + " step=["	+ step + "]" + e.toString(), GuideLogger.ERROR);
				e.printStackTrace();
			}
			
			GuideLogger.printLog("save host:" + userId + " step=[" + step + "]");
			Runnable task_save = new Runnable() {
				public void run() {
					try {
						StepStatusDAO.getInstance().save(userId, step);
						
						//以下是往日志表里打信息
						User logUser=SnsAdapterFactory.getUserAdapter().get(userId);
						int userid=logUser.getId();
						String fromeSource=RegFromSourceLogic.getFromSource(logUser);
						if(fromeSource==null||fromeSource.trim().length()<=0){
						    fromeSource="nosource";
						}
						//下面是AAF逻辑
                        if (fromeSource.endsWith("_AAF")) {
                            //MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
                            
                            if ("".equals("gg")) {
                                String pageid="";
                                if(step==2){
                                    pageid="I0";
                                }
                                if(step==16){
                                    pageid="F0";
                                }
                                insertNewflowLogRecord(userid, 
                                        logUser.getStage(),
                                        fromeSource+"_I0F0", step,pageid);
                                //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                       // "userid:"+userid,(int) MemCacheKeys.day * 31);
                                
                            } else {
                                if (userid%10==1 || userid%10==2) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==4){
                                        pageid="H0";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0H0", step,pageid);
                                    
                                    //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                          //  "userid:"+userid,(int) MemCacheKeys.day * 31);
                                } 
                                else if (userid%10==3 || userid%10==4) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==4){
                                        pageid="H0";
                                    }
                                    if(step==131072){
                                        pageid="Z3";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0Z3H0", step,pageid);
                                    
                                    //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                          //  "userid:"+userid,(int) MemCacheKeys.day * 31);
                                } 
                                else if (userid%10==5 || userid%10==6 ||userid%10==7 || userid%10==8) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==4){
                                        pageid="H0";
                                    }
                                    if(step==131072){
                                        pageid="Z2";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0Z2H0", step,pageid);
                                    
                                    //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                          //  "userid:"+userid,(int) MemCacheKeys.day * 31);
                                } 
                                
                                else if (userid%10==9) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0", step,pageid);
                                    
                                } 
                                
                                else if (userid%10==0) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==131072){
                                        pageid="Z2";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0Z2", step,pageid);
                                    
                                } 
                                
                                
                                else {
                                	GuideLogger.printLog("exception userid:"+userid,GuideLogger.ERROR);
                                	insertGuideLogRecord(logUser.getId(), 
                                            logUser.getStage(),
                                            fromeSource, step);
                                }
                            }
                        }
                        
                        //下面是AAR逻辑
                        else if(fromeSource.endsWith("_AAR")){
                            if ("".equals("gg")) {
                            	 String pageid="";
                                 if(step==2){
                                     pageid="I0";
                                 }
                                 if(step==16){
                                     pageid="F0";
                                 }
                                 if(step==4){
                                     pageid="H0";
                                 }
                                 if(step==131072){
                                     pageid="Z1";
                                 }
                                 insertNewflowLogRecord(userid, 
                                         logUser.getStage(),
                                         fromeSource+"_I0F0Z1H0", step,pageid);
                                
                            } else {
                                if (userid%10==1 || userid%10==2) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==4){
                                        pageid="H0";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0H0", step,pageid);
                                    
                                    //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                          //  "userid:"+userid,(int) MemCacheKeys.day * 31);
                                } 
                                
                                else if (userid%10==3 || userid%10==4 || userid%10==5 || userid%10==6 
                                		||userid%10==7 || userid%10==8 || userid%10==9 || userid%10==0) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==4){
                                        pageid="H0";
                                    }
                                    if(step==131072){
                                        pageid="Z2";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0Z2H0", step,pageid);
                                    
                                    //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                          //  "userid:"+userid,(int) MemCacheKeys.day * 31);
                                } 
                                
                                
                               
                                else {
                                	GuideLogger.printLog("exception userid:"+userid,GuideLogger.ERROR);
                                	insertGuideLogRecord(logUser.getId(), 
                                            logUser.getStage(),
                                            fromeSource, step);
                                }
                            }
                        }
                        
                        //下面是AAP逻辑
                        else if(fromeSource.endsWith("_AAP")){
                            if ("".equals("gg")) {
                            	 String pageid="";
                                 if(step==2){
                                     pageid="I0";
                                 }
                                 if(step==16){
                                     pageid="F0";
                                 }
                                 if(step==4){
                                     pageid="H0";
                                 }
                                 if(step==131072){
                                     pageid="Z1";
                                 }
                                 insertNewflowLogRecord(userid, 
                                         logUser.getStage(),
                                         fromeSource+"_I0F0Z1H0", step,pageid);
                                
                            } else {
                                if (userid%10==1 || userid%10==2) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==4){
                                        pageid="H0";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0H0", step,pageid);
                                    
                                    //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                          //  "userid:"+userid,(int) MemCacheKeys.day * 31);
                                } 
                                
                                else if (userid%10==3 || userid%10==4 || userid%10==5 || userid%10==6 
                                		||userid%10==7 || userid%10==8 || userid%10==9 || userid%10==0) {
                                    String pageid="";
                                    if(step==2){
                                        pageid="I0";
                                    }
                                    if(step==16){
                                        pageid="F0";
                                    }
                                    if(step==4){
                                        pageid="H0";
                                    }
                                    if(step==131072){
                                        pageid="Z2";
                                    }
                                    insertNewflowLogRecord(userid, 
                                            logUser.getStage(),
                                            fromeSource+"_I0F0Z2H0", step,pageid);
                                    
                                    //mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(), 
                                          //  "userid:"+userid,(int) MemCacheKeys.day * 31);
                                } 
                                
                                
                                
                                else {
                                	GuideLogger.printLog("exception userid:"+userid,GuideLogger.ERROR);
                                	insertGuideLogRecord(logUser.getId(), 
                                            logUser.getStage(),
                                            fromeSource, step);
                                }
                            }
                        }
                        
                        
                        
                        else{
                        	insertGuideLogRecord(userId, 
                                    logUser.getStage(),
                                    fromeSource, step);
                        }
                        //打印日志结束
                        
                        
						
					} catch (SQLException e) {
						GuideLogger.printLog("save err host:" + userId + " step=["	+ step + "]" + e.toString(), GuideLogger.ERROR);
						e.printStackTrace();
					}
				}
			};
			RunnableTaskExecutor.getInstance().runTask(task_save);
			return isOK;
		}
		
		//Old code below!
		try {
			int count = StepStatusDAO.getInstance().getCountById(userId);
			int stage = StepStatusLogic.getStepStatus(userId);
			if (count > 0 || stage != 0) {
				MemCacheManager mem = MemCacheFactory.getInstance()
						.getManagerByPoolName(MemCacheKeys.pool_user_space);
				String key = MEM_CACHE_GUIDE_STATUS_ID + userId;
				mem.set(key, Integer.valueOf(step
						| StepStatusLogic.getStepStatus(userId)),
						(int) MemCacheKeys.minute * 30);
				GuideLogger.printLog("host:"+userId+" - "+key);
				//DAO操作变成异步的！
				Runnable taskSave_STATUS = new Runnable() {
					public void run() {
						try {
							StepStatusDAO.getInstance().updateStatus(userId, step);
						} catch (SQLException e) {
							e.printStackTrace();
						}
					}
				};
				RunnableTaskExecutor.getInstance().runTask(taskSave_STATUS);
				
			} else {
				MemCacheManager mem = MemCacheFactory.getInstance()
						.getManagerByPoolName(MemCacheKeys.pool_user_space);
				String key = MEM_CACHE_GUIDE_STATUS_ID + userId;
				mem.set(key, Integer.valueOf(step), (int)
						MemCacheKeys.minute * 30);
				
				//DAO操作变成异步的！
				Runnable taskSave_STATUS = new Runnable() {
					public void run() {
						try {
							StepStatusDAO.getInstance().add(userId, step);
						} catch (SQLException e) {
							e.printStackTrace();
						}
					}
				};
				RunnableTaskExecutor.getInstance().runTask(taskSave_STATUS);
			}
		} catch (Exception e) {
			isOK = false;
			e.printStackTrace(System.err);
		}
		// 更新成功后清空mem cache
		return isOK;
	}

	public static boolean updateStepStatusForMobile(final int userId, final int step){

		try {
			MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_user_space);
			String key = MEM_CACHE_GUIDE_STATUS_ID + userId;
			GuideLogger.printLog("host:" + userId + " - " + key);
			mem.set(key, Integer.valueOf(step | StepStatusLogic.getStepStatus(userId)),	(int) MemCacheKeys.minute * 30);
		} catch (SQLException e) {
			GuideLogger.printLog("save err host:" + userId + " step=["	+ step + "]" + e.toString(), GuideLogger.ERROR);
			e.printStackTrace();
		}
		
		GuideLogger.printLog("save host:" + userId + " step=[" + step + "]");
		Runnable task_save = new Runnable() {
			public void run() {
				try {
					StepStatusDAO.getInstance().save(userId, step);
					
				} catch (SQLException e) {
					GuideLogger.printLog("save err host:" + userId + " step=["	+ step + "]" + e.toString(), GuideLogger.ERROR);
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(task_save);
		return true;

	}
	/**
	 * 取状态值
	 * 
	 * @param userId
	 * @return
	 * @throws SQLException
	 */
	public static int getStepStatus(int userId) throws SQLException {
		int status = 0;
		// 先从mem cache中取
		MemCacheManager mem = MemCacheFactory.getInstance()
				.getManagerByPoolName(MemCacheKeys.pool_user_space);
		String key = MEM_CACHE_GUIDE_STATUS_ID + userId;
		Object obj = mem.get(key);
		try {
			status = ((Integer) obj).intValue();
		} catch (NullPointerException e) {
			GuideLogger.printLog("exception accured - "+status);
		}
		// 如果取不到，把从库中取，并把结果放到mem cache中
		if (obj == null) {
			GuideLogger.printLog("obj is null");
			try {
				status = StepStatusDAO.getInstance().getStatus(userId);
				GuideLogger.printLog("from db :"+status);
			} catch (Exception e) {
				GuideLogger.printLog("db exception accurd:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			mem.set(key, Integer.valueOf(status), (int) MemCacheKeys.minute * 30);
		}
		return status;
	}

	private static String getRedirectUrlForUniv(int userId) {
		String toUrl = "";
		try {
			int step = getStepStatus(userId);
			if ((step == 0) || (step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + "/fillinfodx.do";
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + "/previewdx.do";
			} else if ((step & STEP_IMPORT_MSN) != STEP_IMPORT_MSN) {
				toUrl = Globals.urlGuide + "/regimportmsndx.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		return toUrl;
	}

	/**
	 * 高中学生的引导流程控制器，通过分析高中同学，现在所处的引导位置，返回下个流程节点url
	 * 
	 * @param userId
	 * @return 接下来的引导工作url
	 * @id getRedirectUrlForHighsc
	 * @author wang-tai
	 * @msn tai.wang@opi-corp.com
	 * @date 2008-6-6
	 */
	private static String getRedirectUrlForHighsc(int userId) {
		String toUrl = "";
		try {
			int step = getStepStatus(userId);

			if ((step == 0) || (step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + "/fillinfohs.do";
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + "/previewhs.do";
			}

		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		return toUrl;
	}

	/*
	 * private static String getRedirectUrlForMajor(int userId) { String toUrl =
	 * ""; try { //int step = getStepStatus(userId); if ((step == 0) || (step &
	 * STEP_IMPORT_MSN) != STEP_IMPORT_MSN) { toUrl = Globals.urlGuide +
	 * "/regimportmsnbl.do"; } /*else if ((step & STEP_UPLOAD_HEAD) !=
	 * STEP_UPLOAD_HEAD) { toUrl = Globals.urlGuide + "/portraitbl.do"; } }
	 * catch (Exception e) { e.printStackTrace(System.err); }
	 * 
	 * return toUrl; }
	 */

	/**
	 * 嘿嘿，这个人来访问过这里吗？<br>
	 * 会更新状态，标记为来过
	 * 
	 * @param _userId
	 * @param _here
	 * @return yes or no
	 */
	public static boolean hasVisited(int _userId, int _here) {
		return hasVisited(_userId, _here, true);
	}

	/**
	 * 嘿嘿，这个人来访问过这里吗？<br>
	 * 
	 * @param _userId
	 * @param _here
	 * @param _isUpdate
	 *            在判断没有到过_here的情况后，是否自动跟新状态为以到过
	 * @return yes or no
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午03:29:43
	 */
	public static boolean hasVisited(int _userId, int _here, boolean _isUpate) {
		TimeCost cost = TimeCost.begin("StepStatusLogic.hasVisited");
		boolean yes = true;
		try {
			int step = StepStatusLogic.getStepStatus(_userId);
			cost.endStep("1");
			if ((step & _here) == _here) {
				yes = true;
			} else {
				if (true == _isUpate) {
					StepStatusLogic.updateStepStatus(_userId, _here);
				}
				cost.endStep("2");
				yes = false;
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		} finally {
			cost.endFinally();
		}
		return yes;
	}

	private static boolean needGuide(int userId) {
		
		//二〇〇九年九月一日 开心测试双域名过程中，发现新用户都不走引导
		//原因是这有个ID的判断，这是在远古的5Q时代存在的判断，现在已经用不着了
		//所以给去掉吧。
		//去掉这个判断 得到了庄宏斌的确认
		boolean newUser = true;//(userId > 249579289 && userId < 800000000);
		if (newUser == true
				&& false == hasVisited(userId, STEP_COMPLETE_GUIDE, false))
			return true;
		else
			return false;
		// return true;
	}

	/**
	 * 去相应的引导过程中的页面
	 * 
	 * @param host
	 * @return 返回应去注册连接地址
	 */
	public static String goWhere(User host) {
		outputlog("stepstatuslogic start");
//		outputlog("needGuide ? " + needGuide(host.getId()));
		// 老用户
		if (false == needGuide(host.getId())) {
			GuideLogger.printLog("hostid:" + host.getId() + "- [" + host.getName() + "] is an old user, no guide");
			return "";
		}
		// 需要引导的新用户
		else {
//			outputlog("isMobileUser(host) ? " + isMobileUser(host));
			String fromSource = RegFromSourceLogic.getFromSource(host);
			GuideLogger.printLog("hostid:" + host.getId() + "- [" + host.getName() + "] from source is : "+fromSource);
			
			//2009-11-2 11:27:34
			//为了网吧需求 增加的流程
//			if(fromSource == null){//这个拦不能要，因为后面有手机……
//				MyLogger.printLog("hostid:" + host.getId()+" -> goWhere090729AddStage(host)  because of null fromsource!");
//				return goWhere090729AddStage(host);
//			}

			// 4399 广告注册
			if (StringUtils.equals(RegFromSource.S_4399, fromSource)) {
				GuideLogger.printLog("hostid:" + host.getId()+" -> goWhere4399AD(host)");
				return goWhere090729AddStage(host);
				//return goWhere4399AD(host); 2009-11-25 10:20:30
			}
			// 短表单注册
			else if (StringUtils.equals(RegFromSource.S_SCREG, fromSource)
					|| StringUtils.equals(RegFromSource.S_SIREG, fromSource)) {
				GuideLogger.printLog("hostid:" + host.getId()+" -> goWhere090729AddStage(host)");
				return goWhere090729AddStage(host);
				// return goWhereShortForm(host);
			}
			// g3通道
			else if (StringUtils.equals(RegFromSource.S_G3REG, fromSource)) {
				GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereG3Form(host)");
				return goWhere090729AddStage(host);
			}
			// 手机上注册的用户
			else if (true == isMobileUser(host,fromSource)) {
				//inshion 2011-01-11 16:59:16 改手机！
				/*GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereMobile(host)");
				RegFromSourceLogic.update(host.getId(), RegFromSourceLogic.FS_LEAD_AAO);
				return goWhere090729AddStage(host);*/
				
				//2011-01-20 15:15:23 回滚回来
				GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereMobile(host)");
				return goWhereMobile(host);
			}
			// id注册
			else if (StringUtils.equals(RegFromSource.S_IDREG, fromSource)) {
				GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereIDForm(host)X -> new guide");
				return goWhere090729AddStage(host);
				//return goWhereIDForm(host);
			}
			// Page注册
			else if (StringUtils.equals(RegFromSource.S_PAGE,
					getPageSource(fromSource))) {
				GuideLogger.printLog("hostid:" + host.getId()+" -> goWherePageForm(host)");
				try {
					return goWherePageForm(host, StringUtils.split(StringUtils
							.trimToEmpty(fromSource), ":")[1]);
				} catch (Exception e) {
				}
			}

			// 渠道的！
			else if (hasFromSource(fromSource)){ //又有FromSource，但又没有被前面的If拦住
//				if(isConfiguredChannal(fromSource)){
//					GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereAppChannalNoGuide(host)");
//					return goWhereAppChannalNoGuide(host);
//				}
//				else 
				if(StringUtils.equals(fromSource, "S_FARMREG")){
					GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereFlashFarm(host)");
					return goWhereFlashFarm(host);
				}
				else if(StringUtils.equals(fromSource, "S_COOKREG")){
					GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereFlashCook(host)");
					return goWhereFlashCook(host);
				}
				else{
					List<String> fromlist = RegFromSourceLogic.getFromSoureList(fromSource);
					if(fromlist.contains("S_FARMREG")){
						GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereFlashFarm(host)");
						return goWhereFlashFarm(host);
					}
					else if(fromlist.contains(RegFromSourceLogic.FS_MAZIMIANGUAN)){
						GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereAppMazimianguan(host)");
						return goWhereAppMazimianguan(host);
					}
					else if(fromlist.contains("S_COOKREG")){
						GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereFlashCook(host)");
						return goWhereFlashCook(host);
					}
					else if(fromlist.contains("NETBAR")){
						GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereNetbar(host)");
						return goWhereNetbar(host);
					}
					else if(fromlist.contains(RegFromSourceLogic.FS_NETBARXFIVE)){
						GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereNetbar(host)");
						return goWhereNetbar(host);
					}
					else{
						GuideLogger.printLog("hostid:" + host.getId()+" -> goWhere090729AddStage(host)");
						return goWhere090729AddStage(host);
					}
				}
			}
			// web上注册的用户
			GuideLogger.printLog("hostid:" + host.getId()+" -> goWhereWeb(host)");
			
			
			return goWhere090729AddStage(host);
		}
	}
	
	@SuppressWarnings("unused")
	private static boolean isConfiguredChannal(String fromSource){
		GuideLogger.printLog("fromSource:" + fromSource);
		if(fromSource == null) return false;
		return A090729Util.hasPageKey(fromSource.trim());
	}
	
	private static boolean hasFromSource(String fromSource){
		GuideLogger.printLog("fromSource:" + fromSource);
		if(fromSource == null) return false;
		if("".equals(fromSource.trim())) return false;
		return true;
	}
	
	private static String getPageSource(String fromSource) {
		try {
			return StringUtils.split(StringUtils.trimToEmpty(fromSource), ":")[0];
		} catch (Exception e) {
			return "";
		}
	}

	/**
	 * page跳转
	 * 
	 * @param host
	 * @param uuid
	 * @return
	 */
	private static String goWherePageForm(User host, String uuid) {
		updateStepStatus(host.getId(), STEP_COMPLETE_GUIDE);
		return Globals.urlPage+"/type/creator?uuid=" + uuid;
	}

	private static final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_user_space);
	private static final String merge_key_prefix = "merge_send_";
	/**
	 * 090729添加。重新加上阶段的引导流程。
	 * 
	 * @param host
	 * @return
	 */
	private static String goWhere090729AddStage(User host) {
		//2009-11-23 11:01:30 只牵十分之一过去
		if(GuideUtil.needGoToXFiveUser(host)){
			//发验证码信 糯米inshion
			if("nuomi.com".equals(host.getDomain())){
				if(StepProcessLogic.getInstance().isNeedMergeUser(host)){
					incMergeMemcacheValue(host);
					GuideLogger.printLog("hostid:"+ host.getId() + " needmerge");
				}
				else{
					GuideLogger.printLog("hostid:"+ host.getId() + " neednotmerge");
				}
			}
			else{
				GuideLogger.printLog("hostid:"+ host.getId() + " notnuomi");
			}
			
			return ""+Globals.urlGuide + "/lead";
		}
		/*
		//2009-11-19 13:41:41 牵到Rose框架
		if("".equals("")){
			return "lead";
		}
		*/
		GuideLogger.printLog("hostid:"+ host.getId() + " - start");
		String toUrl = "";
		try {
			boolean less17 = BirthdayLogic.getInstance().less17age(host);
			int step = getStepStatus(host.getId());
			GuideLogger.printLog("hostid:"+ host.getId() +" [StepStatus]="+step);
			if (step == 0 || (step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_FILL_INFO;
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_PREVIEW;
			} else if ((step & STEP_IMPORT_MSN) != STEP_IMPORT_MSN && !less17) {
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_IMPORT_MSN;
			} else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		GuideLogger.printLog("hostid:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}

	private static void incMergeMemcacheValue(User host) {
		Integer sendv = (Integer)mem.get(merge_key_prefix+host.getId());
		if(sendv == null){
			boolean ret = mem.set(merge_key_prefix, 1, (int)MemCacheKeys.day * 7);
			GuideLogger.printLog("hostid:"+ host.getId() +" set_ret:"+ret);
		}
		else{
			long ret = mem.incr(merge_key_prefix+host.getId());
			GuideLogger.printLog("hostid:"+ host.getId() +" inc_ret:"+ret);
		}
	}
	private static String goWhereNetbar(User host) {
		GuideLogger.printLog("hostid:"+ host.getId() + " - start");
		if("".equals("")){
			return ""+Globals.urlGuide + "/lead";
		}
		String toUrl = "";
		try {
			int step = getStepStatus(host.getId());
			GuideLogger.printLog("hostid:"+ host.getId() +" [StepStatus]="+step);
			if (step == 0 || (step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + "/finb.do";
			} 
//			else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
//				toUrl = Globals.urlGuide + "/pvnb.do";
//			} 
//			else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
//				toUrl = Globals.urlGuide + "/ponb.do";
//			}
		} catch (Exception e) {
			GuideLogger.printLog("hostid:"+ host.getId() + "  error"+e.toString(),GuideLogger.ERROR);
		}
		GuideLogger.printLog("hostid:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}
	
	private static String goWhereAppMazimianguan(User host){
		GuideLogger.printLog("hostid:"+ host.getId() + " - start");
		return ""+Globals.urlGuide + "/lead?from=mzmg";
	}
	private static final String APP_GUIDE_PAGE = "/af.do";//app farm flash
	private static String goWhereFlashFarm(User host){
		GuideLogger.printLog("hostid:"+ host.getId() + " - start");
		//2009-12-9 16:00:09 牵到Rose框架
		if("".equals("")){
			return ""+Globals.urlGuide + "/lead?from=farm";//"lead?from=farm";
		}
		//AppUserFacade.getInstance().installHappyFarm(host,23163);
		String toUrl = "";
		try {
			int step = getStepStatus(host.getId());
			GuideLogger.printLog("hostid:"+ host.getId() +" [StepStatus]="+step);
			if(step == 0 || (step & STEP_FLASH_APP_OPEN) != STEP_FLASH_APP_OPEN){
				toUrl = Globals.urlGuide + APP_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+1);
			} else if ((step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + APP_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+2);
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + APP_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+3);
			} else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
				toUrl = Globals.urlGuide + APP_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+4);
			} else if ((step & STEP_APP) != STEP_APP){
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_APP;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+5);
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		GuideLogger.printLog("hostid:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}
	
	private static final String APP_COOK_GUIDE_PAGE = "/acook.do";//app cook flash
	private static String goWhereFlashCook(User host){
		GuideLogger.printLog("hostid:"+ host.getId() + " - start");
		//2009-12-15 12:47:53 牵到Rose框架
		if("".equals("")){
			return ""+Globals.urlGuide + "/lead?from=cook";
		}
		//AppUserFacade.getInstance().installHappyFarm(host,23163);
		String toUrl = "";
		try {
			int step = getStepStatus(host.getId());
			GuideLogger.printLog("hostid:"+ host.getId() +" [StepStatus]="+step);
			if(step == 0 || (step & STEP_FLASH_APP_OPEN) != STEP_FLASH_APP_OPEN){
				toUrl = Globals.urlGuide + APP_COOK_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+1);
			} else if ((step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + APP_COOK_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+2);
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + APP_COOK_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+3);
			} else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
				toUrl = Globals.urlGuide + APP_COOK_GUIDE_PAGE;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+4);
			} else if ((step & STEP_APP) != STEP_APP){
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_APP;
				GuideLogger.printLog("hostid:"+ host.getId() +" [Step]="+5);
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		GuideLogger.printLog("hostid:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}
	/**
	 * 对应090821的流程。实际上是用来换线上流程的。
	 * 
	 * @param host
	 * @return
	 */
	@SuppressWarnings("unused")
	private static String goWhereApp(User host) {
		GuideLogger.printLog("hostid:"+ host.getId() + " - start",GuideLogger.ERROR);
		String toUrl = "";
		try {
			boolean less17 = BirthdayLogic.getInstance().less17age(host);
			int step = getStepStatus(host.getId());
			GuideLogger.printLog("hostid:"+ host.getId() +" [StepStatus]="+step);
			if(step == 0 ||(step & STEP_APP) != STEP_APP){
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_APP;
			} else if ((step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + "/fi.do";
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_PREVIEW;
			} else if ((step & STEP_IMPORT_MSN) != STEP_IMPORT_MSN && !less17) {
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_IMPORT_MSN;
			} else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		GuideLogger.printLog("hostid:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}
	
	@SuppressWarnings("unused")
	private static String goWhereAppChannalNoGuide(User host) {
		GuideLogger.printLog("hostid:"+ host.getId() + " - start");
		String toUrl = "";
		try {
//			boolean less17 = BirthdayLogic.less17age(host);
			int step = getStepStatus(host.getId());
			GuideLogger.printLog("hostid:"+ host.getId() +" [StepStatus]="+step);
			if(step == 0 || (step & STEP_APP) != STEP_APP){
				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_APP;
				GuideLogger.printLog("hostid:"+ host.getId() +" only this step ");
			} 
			//20090827 据说渠道用户都不走引导了。所以把这些都去掉了。
//			else if ((step & STEP_FILLINFO) != STEP_FILLINFO) {
//				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_FILL_INFO;
//			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
//				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_PREVIEW;
//			} else if ((step & STEP_IMPORT_MSN) != STEP_IMPORT_MSN && !less17) {
//				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_IMPORT_MSN;
//			} else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
//				toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
//			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		GuideLogger.printLog("hostid:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}

	@SuppressWarnings("unused")
	private static String goWhereIDForm(User host) {
		String toUrl = "";
		try {
			int step = getStepStatus(host.getId());
			if ((step == 0) || (step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + "/fillinfo-sf-09f.do";
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + "/preview-sf.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		return toUrl;
	}

	@SuppressWarnings("unused")
	private static String goWhereShortForm(User host) {
		String toUrl = "";
		try {
			boolean less17 = BirthdayLogic.getInstance().less17age(host);
			int step = getStepStatus(host.getId());
			if ((step == 0) || (step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + "/fill-info-ns.do";
			} else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = Globals.urlGuide + "/preview-ns.do";
			} else if ((step & STEP_IMPORT_MSN) != STEP_IMPORT_MSN && !less17) {
				toUrl = Globals.urlGuide + "/import-msn-ns.do";
			} else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
				toUrl = Globals.urlGuide + "/head-upload-ns.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		return toUrl;
	}

	/**
	 * @param host
	 * @return
	 */
	@SuppressWarnings("unused")//2009-11-23 10:58:06 去掉
	private static String goWhereG3Form(User host) {
		String toUrl = "";
		try {
			int step = getStepStatus(host.getId());
			if ((step == 0) || (step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = Globals.urlGuide + "/fillinfo-sf-g3.do";
			}
			// } else if ((step & STEP_IMPORT_MSN) != STEP_IMPORT_MSN) {
			// toUrl = Globals.urlGuide + "/importmsn.do";
			// }
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		return toUrl;
	}

	/**
	 * 4399 快速注册用户 去相应的引导过程中的页面
	 * 
	 * @param _host
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午07:02:56
	 */
	@SuppressWarnings("unused")
	private static String goWhere4399AD(User _host) {
		switch (UserAdapter.get(_host).getStage()) {
		case Stage.STAGE_HEIGHSCHOOL:
			return getRedirectUrlForHS4399(_host.getId());
		case Stage.STAGE_UNIV:
			return getRedirectUrlForUniv4399(_host.getId());
		case Stage.STAGE_NONE:
		case Stage.STAGE_MAJOR:
			return getRedirectUrlForMajor4399(_host.getId());
		default:
			return "";
		}
	}

	private static String getRedirectUrlForMajor4399(int id) {
		String toUrl = "";
		try {
			int step = getStepStatus(id);
			if ((step == 0)
					|| (step & STEP_4399_FILLINFO) != STEP_4399_FILLINFO) {
				toUrl = Globals.urlGuide + "/quick-fillinfo-bl.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return toUrl;
	}

	private static String getRedirectUrlForUniv4399(int id) {
		String toUrl = "";
		try {
			int step = getStepStatus(id);
			if ((step == 0)
					|| (step & STEP_4399_FILLINFO) != STEP_4399_FILLINFO) {
				toUrl = Globals.urlGuide + "/quick-fillinfo-un.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return toUrl;
	}

	private static String getRedirectUrlForHS4399(int id) {
		String toUrl = "";
		try {
			int step = getStepStatus(id);
			if ((step == 0)
					|| (step & STEP_4399_FILLINFO) != STEP_4399_FILLINFO) {
				toUrl = Globals.urlGuide + "/quick-fillinfo-hs.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return toUrl;
	}

	/**
	 * web注册用户 去相应的引导过程中的页面
	 * 
	 * @param _host
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午02:58:29
	 */
	@SuppressWarnings("unused")
	private static String goWhereWeb(User _host) {
		if (UserAdapter.get(_host).getStage() == Stage.STAGE_UNIV) {
			return StepStatusLogic.getRedirectUrlForUniv(_host.getId());
		} /*
		 * else if (_host.getStage() == Stage.STAGE_MAJOR) { return
		 * StepStatusLogic.getRedirectUrlForMajor(_host.getId()); } else if
		 * (_host.getStage() == Stage.STAGE_NONE) { return
		 * StepStatusLogic.getRedirectUrlForMajor(_host.getId()); }
		 */else if (UserAdapter.get(_host).getStage() == Stage.STAGE_HEIGHSCHOOL) {
			return StepStatusLogic.getRedirectUrlForHighsc(_host.getId());
		} else {
			return "";
		}
	}

	/**
	 * 手机注册用户 去相应的引导过程中的页面
	 * 
	 * @param _host
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-11-28 - 下午07:44:11
	 */
	private static String goWhereMobile(User _host) {
//		outputlog("host.getStage( ? " + UserAdapter.get(_host).getStage());
		//outputlog("update Stage  ");
		GuideLogger.printLog(" host:"+_host.getId()+" stage:"+_host.getStage());
		//switch (UserAdapter.get(_host).getStage()) {
		switch (_host.getStage()) {
		case Stage.STAGE_HEIGHSCHOOL:
			return getRedirectUrlForHSMobile(_host.getId());
		case Stage.STAGE_UNIV:
			return getRedirectUrlForUnivMobile(_host.getId());
		case Stage.STAGE_NONE:
		case Stage.STAGE_MAJOR:
		default:
			return getRedirectUrlForMajorMobile(_host.getId());
		}
	}

	/**
	 * 手机注册的web引导 高中阶段
	 * 
	 * @param id
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午12:05:09
	 */
	private static String getRedirectUrlForHSMobile(int _userId) {
		String toUrl = "";
		try {
			int step = getStepStatus(_userId);
			if ((step == 0)
					|| (step & STEP_FILLINFO_STAGE_MOBILE) != STEP_FILLINFO_STAGE_MOBILE) {
				toUrl = Globals.urlGuide + "/fillinfost.do";
			} else if ((step & STEP_FILLINFO_MOBILE) != STEP_FILLINFO_MOBILE) {
				toUrl = Globals.urlGuide + "/fillinfomg.do";
			}/*
			 * else if ((step & STEP_VERIFY_MOBILE) != STEP_VERIFY_MOBILE) {
			 * toUrl = Globals.urlGuide + "/verifymail.do"; }
			 */
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		return toUrl;
	}

	/**
	 * 手机注册的web引导 大学阶段
	 * 
	 * @param _userId
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午12:08:08
	 */
	private static String getRedirectUrlForUnivMobile(int _userId) {
		outputlog("getRedirectUrlForUnivMobile start");
		String toUrl = "";
		try {
			int step = getStepStatus(_userId);
			if ((step == 0)
					|| (step & STEP_FILLINFO_STAGE_MOBILE) != STEP_FILLINFO_STAGE_MOBILE) {
				toUrl = Globals.urlGuide + "/fillinfost.do";
			} else if ((step & STEP_FILLINFO_MOBILE) != STEP_FILLINFO_MOBILE) {
				toUrl = Globals.urlGuide + "/fillinfomd.do";
			} else if ((step & STEP_VERIFY_MOBILE) != STEP_VERIFY_MOBILE) {
				toUrl = Globals.urlGuide + "/verifymail.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		outputlog("toUrl ? " + toUrl);
		return toUrl;
	}

	/**
	 * 手机注册的web引导 白领阶段
	 * 
	 * @param id
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午12:08:22
	 */
	private static String getRedirectUrlForMajorMobile(int _userId) {
		String toUrl = "";
		try {
			int step = getStepStatus(_userId);
			if ((step == 0)
					|| (step & STEP_FILLINFO_STAGE_MOBILE) != STEP_FILLINFO_STAGE_MOBILE) {
				toUrl = Globals.urlGuide + "/fillinfost.do";
			} else if ((step & STEP_FILLINFO_MOBILE) != STEP_FILLINFO_MOBILE) {
				toUrl = Globals.urlGuide + "/fillinfomb.do";
			} else if ((step & STEP_VERIFY_MOBILE) != STEP_VERIFY_MOBILE) {
				toUrl = Globals.urlGuide + "/verifymail.do";
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}

		return toUrl;
	}

	/**
	 * 是否为手机注册用户
	 * 
	 * @param host
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-11-28 - 下午07:16:00
	 * @param fromSource 
	 */
	private static boolean isMobileUser(User host, String fromSource) {
		//2010-4-14 10:23:02 把开心和人人都统一了
		/*if (!isRenrenUser(host)){
			return false;
		}*/
		// 因为手机注册时候没有设置性别，现在区别手机注册和web注册的方式通过这一点,与是否走过手机引导
	    // 2010-9-6 17:41:59 不光如此，要判断IMOP的情况了……
	    // 2011-05-19 21:07:53不光如此要判断NUOMI的情况了……
	    if(!MyStringUtils.contains(host.getDomain(), "renren.com")){
	        return false;
	    }
	    // 为了小站注册 2011-08-16 10:28:04
	    if(MyStringUtils.contains(fromSource, "_")){ //_ABD 
	        return false;
	    }
		int step;
		try {
			step = getStepStatus(host.getId());
		} catch (SQLException e) {
			e.printStackTrace();
			step = 0;
		}
		if (StringUtils.isEmpty(UserAdapter.get(host).getGender())
				|| (step & STEP_FILLINFO_MOBILE) == STEP_FILLINFO_MOBILE) {
			return true;
		} else {
			return false;
		}
	}

	private static void outputlog(String str) {
	}
	
	@SuppressWarnings("unused")
	private static boolean isRenrenUser(User host){
		if("renren.com".equals(host.getDomain()) && "renren.com".equals(OpiConstants.domain.toString())){
			return true;
		}
		else{
			return false;
		}
	}
	
	public static List<String> doTest(int step) {
		List<String > list = new ArrayList<String>();
//		int step = Integer.valueOf("111111111111111111", 2);
		if ((step & STEP_FILLINFO) == STEP_FILLINFO) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_FILL_INFO;
			list.add(1+" STEP_FILLINFO "+STEP_FILLINFO);
		} 
		if ((step & STEP_PREVIEW) == STEP_PREVIEW) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_PREVIEW;
			list.add(2+" STEP_PREVIEW "+STEP_PREVIEW);
		}
		if ((step & STEP_IMPORT_MSN) == STEP_IMPORT_MSN ) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_IMPORT_MSN;
			list.add(3+" STEP_IMPORT_MSN "+STEP_IMPORT_MSN);
		}
		if ((step & STEP_UPLOAD_HEAD) == STEP_UPLOAD_HEAD) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(4+" STEP_UPLOAD_HEAD "+STEP_UPLOAD_HEAD);
		}
		if ((step & STEP_GUIDE) == STEP_GUIDE) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(5+" STEP_GUIDE "+STEP_GUIDE);
		}
		if ((step & STEP_FILLINFO_MOBILE) == STEP_FILLINFO_MOBILE) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(6+" STEP_FILLINFO_MOBILE "+STEP_FILLINFO_MOBILE);
		}
		if ((step & STEP_VERIFY_MOBILE) == STEP_VERIFY_MOBILE) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(7+" STEP_VERIFY_MOBILE "+STEP_VERIFY_MOBILE);
		}
		
		if ((step & STEP_COMPLETE_GUIDE) == STEP_COMPLETE_GUIDE) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(8+" STEP_COMPLETE_GUIDE "+STEP_COMPLETE_GUIDE);
		}
		if ((step & STEP_4399_FILLINFO) == STEP_4399_FILLINFO) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(9+" STEP_4399_FILLINFO "+STEP_4399_FILLINFO);
		}
		if ((step & STEP_FILLINFO_STAGE_MOBILE) == STEP_FILLINFO_STAGE_MOBILE) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(10+" STEP_FILLINFO_STAGE_MOBILE "+STEP_FILLINFO_STAGE_MOBILE);
		}
		if ((step & STEP_APP) == STEP_APP) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(11+" STEP_APP "+STEP_APP);
		}
		if ((step & STEP_FLASH_APP_OPEN) == STEP_FLASH_APP_OPEN) {
			//toUrl = Globals.urlGuide + UrlFor090729AddStage.URL_UPLOAD_HEAD;
			list.add(12+" STEP_FLASH_APP_OPEN "+STEP_FLASH_APP_OPEN);
		}
		if ((step & IStepStatusDefinition.STEP_MOREFRIEND) == IStepStatusDefinition.STEP_MOREFRIEND) {
			list.add(13+" STEP_MOREFRIEND "+IStepStatusDefinition.STEP_MOREFRIEND);
		}
		return list;
	}

}

interface UrlFor090729AddStage {
	// public final String URL_FILL_INFO_DX = "/fi-090729-dx.do";
	// public final String URL_FILL_INFO_ZX = "/fi-090729-zx.do";
	// public final String URL_FILL_INFO_BL = "/fi-090729-bl.do";
	// public final String URL_FILL_INFO_QT = "/fi-090729-qt.do";
	public final String URL_FILL_INFO = "/fi-090729.do";
	public final String URL_PREVIEW = "/pv-090729.do";
	public final String URL_IMPORT_MSN = "/im-090729.do";
	public final String URL_UPLOAD_HEAD = "/hu-090729.do";
	public final String URL_APP = "/gapp.do"; 

}

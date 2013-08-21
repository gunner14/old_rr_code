package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import java.io.IOException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.lang.StringUtils;
import org.json.JSONArray;
import org.json.JSONObject;

import Ice.TimeoutException;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.component.friends.selector.SelectorContext;
import com.xiaonei.platform.component.friends.selector.filter.FriendsPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.GroupFilter;
import com.xiaonei.platform.component.friends.selector.filter.IPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.NetworkFilter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WFriendDistCount;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveIDsPreviewLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.impl.WFriendDistAdapter;


/**
 * AjaxGetGuideXFiveRecommendFriendsController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-25 添加说明 
 */
@LoginRequired
@Path("ajxFs")
public class AjaxGetGuideXFiveRecommendFriendsController{
    
    private static final int Max_Thread_N = 88;
    private static final int Redo_Times = 33;
    private static final int Sleep_Time  = 14;
    private static AtomicInteger Thread_Count = new AtomicInteger(0);
    private static final int Max_Friend_Count_N = 45;
    private static ExecutorService exetorspool = Executors.newCachedThreadPool();
    
    static class CharacterChain implements Chain {

        List<Command> commands = new ArrayList<Command>();

        public void addCommand(Command com) {
            commands.add(com);
        }

        public boolean execute(Context con) throws Exception {
            for (Command command : commands) {
                if (command.execute(con) == false) {
                    return false;
                }
            }
            return true;
        }
    }

    @Get
    @Post
    @SuppressWarnings({ "unchecked" })
    public String index(Invocation inv) {
        
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        
        String skip = request.getParameter("skip");
        if(MyStringUtils.equals("1", skip)){
            return "@{\"candidate\":[]}";
        }

        User host = BaseThreadUtil.currentHost(request);
        String jstring = request.getParameter("p");// json参数
        GuideLogger.printLog(jstring);
        JSONObject param;
        if ((jstring == null) || (jstring.trim().length() == 0)) {
            System.err.println("AjaxGetFriendsController:Error JSONObject form1.");
            return null;
        }
        try {
            param = new JSONObject(jstring);
        } catch (ParseException e) {
            System.err.println("AjaxGetFriendsController:Error JSONObject form2.");
            return null;
        }
        JSONObject callerJParam;
        try {
            callerJParam = param.getJSONObject("param");
        } catch (NoSuchElementException e1) {
            System.err.println("AjaxGetFriendsController:Error JSONObject form3.");
            return null;
        }
        Iterator<String> iter = callerJParam.keys();// 组装成map
        Map<String, String> callerParam = new HashMap<String, String>();

        while (iter.hasNext()) {
            String key = iter.next();
            callerParam.put(key, callerJParam.getString(key));
            GuideLogger.printLog(key);
        }

        //此处不能用host.了，需要换乘页面上设置的相关资料信息
        List<Integer> candidate = getCandidate(host, callerParam, request);// 取得候选id
        
        //List<WUserCache> ucl = SnsAdapterFactory.getUserCacheAdapter().getUsersCacheList(candidate);
        Map<Integer, WUserCache> ucl = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(candidate);

        if (ucl == null) {
            ucl = new HashMap<Integer, WUserCache>();
        }

        CharacterChain filter = new CharacterChain();

        JSONObject jo = new JSONObject();
        if (!param.optBoolean("init")) {// 查询操作
            SelectorContext context = new SelectorContext();
            context.setHost(host);
            if (param.optString("group").trim().length() != 0) {
                filter.addCommand(new GroupFilter());
                context.setGroup(param.getString("group"));
            }
            if (param.optString("net").trim().length() != 0) {
                filter.addCommand(new NetworkFilter());
                context.setNetwork(Integer.parseInt(param.getString("net")));
            }
            if ((param.optString("query").trim().length() != 0)
                    && (param.optString("qkey").trim().length() != 0)) {
                filter.addCommand(getPinyinFilter());
                context.setQuery(param.getString("query").trim());
                context.setKey(param.getString("qkey").trim());
                context.setLimit(param.optInt("limit"));
            }

            for (int i = ucl.size() - 1; i >= 0; i--) {
                if (ucl.get(i) == null) {
                    ucl.remove(i);
                    continue;
                }

                try {
                    context.setUser(ucl.get(i));
                    if (!filter.execute(context)) {
                        ucl.remove(i);
                    }
                } catch (Exception e) {}
            }

        } else {// 初始化操作
            if (param.optBoolean("group")) {// 取host的分组
                JSONArray ja = getHostGroup(host.getId());
                if (ja != null) {
                    jo.put("groups", ja);
                }
            }
            if (param.optBoolean("net")) {// 取好友所在的网络
                JSONArray ja = getHostNet(host.getId());
                if (ja != null) {
                    jo.put("net", ja);
                }
            }
            // 查询用的key
            Map<Integer, String> candidateMap = new HashMap<Integer, String>();
            for (WUserCache userCache : ucl.values()) {
                if (userCache != null) {
                    candidateMap.put(userCache.getId(), userCache.getName());
                }
            }
            String key = getPinyinFilter().buildPinyinData(candidateMap);
            jo.put("qkey", key);
        }
        // 返回数据
        JSONArray retCandidate = new JSONArray();
        
        for (int curid : candidate) {
        	WUserCache userCache = ucl.get(curid);
        	if(userCache != null){
        		JSONObject joinner = new JSONObject();
                if (param.optBoolean("uid")) {
                    joinner.put("id", userCache.getId());
                }
                if (param.optBoolean("uhead")) {
                    joinner.put("head", userCache.getTiny_Url());
                }
                if (param.optBoolean("uname")) {
                    joinner.put("name", userCache.getName());
                }
                
                try {
					joinner.put("netName", userCache.getUnivName());
				} catch (NullPointerException e) {
					GuideLogger.printLog(" host:"+host.getId()+" cid:"+curid+" err:"+e.toString(),GuideLogger.ERROR);
					e.printStackTrace();
				}
                retCandidate.put(joinner);
        	}
			
		}

        jo.put("candidate", retCandidate);
        return writeAjax(response, jo.toString());
    }

    protected JSONArray getHostGroup(int hostId) {
        List<String> groups = null;
		try {
			groups = SnsAdapterFactory.getBuddyGroupAdapter().getGroupList(hostId);
			//groups = BuddyGroupAdapter.getInstance().getGroupList(hostId);
		} catch (Exception e) {
			GuideLogger.printLog("getUniversityResults host:"+hostId+" err:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
        JSONArray ja = new JSONArray();
        if(groups == null){
        	return ja;
        }
        else{
        	for (String group : groups) {
        		ja.put(group);
        	}
        	return ja;
        }
    }

    private String writeAjax(final HttpServletResponse response, final String str) {
        try {
            response.setContentType("text/html; charset=UTF-8");
            response.getWriter().print(str);
            response.getWriter().flush();
            response.getWriter().close();
        } catch (IOException e) {
        	GuideLogger.printLog(" e:"+e.toString(),GuideLogger.ERROR);
        } finally {
        	
        }
        return "@";
    }

    @SuppressWarnings("deprecation")
    protected JSONArray getHostNet(int hostId) {
        List<WFriendDistCount> univlist = WFriendDistAdapter.getInstance().getFriendDistListAll(
                hostId);
        if ((univlist != null) && !univlist.isEmpty()) {
            // 去掉0网络
            JSONArray ja = new JSONArray();
            for (int i = 0; i < univlist.size(); i++) {
                JSONObject joinner = new JSONObject();
                if (univlist.get(i).getNetworkId() == 0) {
                    univlist.remove(i);
                    break;
                } else {
                    joinner.put("id", univlist.get(i).getNetworkId());
                    joinner.put("name", univlist.get(i).getNetworkName());
                }
                ja.put(joinner);
            }
            return ja;
        }
        return null;
    }

    protected List<Integer> getCandidate(final User host, Map<String, String> params, final HttpServletRequest request) {
    	GuideTimeCost cost = GuideTimeCost.begin();
        GuideLogger.printLog("hostid:"+ host.getId()+ " - start");
        final List<Integer> hostFriend = new ArrayList<Integer>();
        cost.endStep();
        //Random rand = new Random();
        final int count = Max_Friend_Count_N;
        String ps = params.get("type");
        
        char pstage = StringUtils.isNotEmpty(ps) ? ps.toUpperCase().charAt(0): 'U';
        GuideLogger.printLog("current stage is:"+pstage);
        boolean isfull = false;
        switch(pstage){
            case 'U': //university
            	GuideLogger.printLog("user select stage is:" + pstage);                
            	getUniversityResults(count, host, hostFriend);
                break;
                
            case 'H': //high school
                GuideLogger.printLog("user select stage is:" + pstage);
                getHighScgoolResults(count, host, hostFriend);
                break;
                
            case 'J': //junior high school
                GuideLogger.printLog("user select stage is:" + pstage);
                getJuniorSchoolResults(count, host, hostFriend);
                break;
                
            case 'E': //elementary
                GuideLogger.printLog("user select stage is:" + pstage);      
                getElementaryResults(count, host, hostFriend);
                break;
                
            case 'W'://worker
                GuideLogger.printLog("user select stage is:" + pstage);   
                getWorkResults(count, host, hostFriend);
                break;
                
            case 'R': //region
                GuideLogger.printLog("user select stage is:" + pstage);    
                getRegionResult(count, host, hostFriend);   
                break;
                
            case 'I': //IP, have if check
                GuideLogger.printLog("user select stage is:" + pstage);                
                if(Thread_Count.get() < Max_Thread_N){
                    inc();
                    Thread addtask7 = new Thread(){                        
                        @Override
                        public void run() {
                            try {
                                getSameIpResults(count, host, hostFriend, request);                                
                            } catch (Exception e) {
                                GuideLogger.printLog("getUniversityResults host:"+host.getId(),GuideLogger.ERROR);
                            } finally {
                                dec();
                            }
                        }
                    };                    
                    exetorspool.execute(addtask7);
                }
                else{
                	isfull = true;
                    GuideLogger.printLog("Thread_COUNT full!"+Thread_Count,GuideLogger.WARN);
                }                
                cost.endStep();
                break;
                
            case 'P': //IP,no if check
                GuideLogger.printLog("user select stage is:" + pstage);
                if(Thread_Count.get() < Max_Thread_N){
                    inc();
                    Thread addtask8 = new Thread(){                        
                        @Override
                        public void run() {
                            try {
                                getSameIpPureResults(count, host, hostFriend, request);                                                               
                            } catch (Exception e) {
                                GuideLogger.printLog("getUniversityResults host:"+host.getId(),GuideLogger.ERROR);
                            } finally {
                                dec();
                            }
                        }
                    };                    
                    exetorspool.execute(addtask8);
                }
                else{
                	isfull = true;
                    GuideLogger.printLog("Thread_COUNT full!"+Thread_Count,GuideLogger.WARN);
                }                
                cost.endStep();
                break;
            case 'T': //TechSchool College
            	GuideLogger.printLog("user select stage is:" + pstage);
            	getTechSchoolResults(count, host, hostFriend);                                                              
            	cost.endStep();
            	break;
            default://qita
                GuideLogger.printLog("user select stage is:" + pstage);

                break;            
        }
        
        if(isfull){
        	GuideLogger.printLog("getCandidate error host:"+host.getId()+" - THREADFULLRETURN");
        }
        else{
        	int i = 0;
        	while (++i < Redo_Times) {
        		if (hostFriend.size() < Max_Friend_Count_N) {
        			try {
        				Thread.sleep(Sleep_Time);
        			} catch (InterruptedException e) {
        				e.printStackTrace();
        			}
        		} else {
        			GuideLogger.printLog("i=" + i + " NORMALRETURN " + hostFriend.size() + " Thread C:"+Thread_Count);
        			cost.endStep();
        			cost.endFinally();
        			return hostFriend;
        		}
        	}
        }
        
        cost.endStep();        
        
        {
            GuideLogger.printLog("getCandidate host:"+ host.getId()+ " - TIMEOUTRETURN -"+" get count:"+hostFriend.size());
        }
        cost.endStep();
        cost.endFinally();
        return hostFriend;
    }

    /**
     * 大学推人
     * @param count
     * @param host
     * @param hostFriend
     */
    private void getUniversityResults(int count, User host, List<Integer>hostFriend){
        List<Integer> listU = null;
        //university school
        listU = GuideXFiveIDsPreviewLogic.getInstance().getUniversityResults(count, host);
        try {
        	if(listU != null){
        		List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
        		if(friendlist != null){
        			listU.removeAll(friendlist);
        		}
        		listU.remove(host.getId());
        	}
			
		} catch (TimeoutException e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		} catch (IndexOutOfBoundsException e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
		} catch(Exception e){
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
        
        if(listU!=null){
            add(hostFriend,listU);
            GuideLogger.printLog("UniversityHighschool University Friends:"+listU.size()+" - host:"+host.getId());
        }        
    }
    
    /**
     * 高中推人
     * @param count
     * @param host
     * @param hostFriend
     */
    private void getHighScgoolResults(int count, User host, List<Integer>hostFriend){
        List<Integer> listH=null;
        //high school
        listH = GuideXFiveIDsPreviewLogic.getInstance().getHighschoolResults(count, host);
        try {
            if(listH != null){
                List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
                if(friendlist != null){
                    listH.removeAll(friendlist);
                }
                listH.remove(host.getId());
            }
            
        } catch (TimeoutException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch (IndexOutOfBoundsException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch(Exception e){
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        
        if(listH!=null){
            add(hostFriend,listH);
            GuideLogger.printLog("HighAndJuniorScgoolResults High Friends:"+listH.size()+" - host:"+host.getId());
        }        
    }
    
    /**
     * 中专推人
     * @param count
     * @param host
     * @param hostFriend
     */
    private void getTechSchoolResults(int count, User host, List<Integer>hostFriend){
        List<Integer> listJ = null;
        listJ = GuideXFiveIDsPreviewLogic.getInstance().getCollegeResults(count, host);
        
        try {
            if(listJ != null){
                List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
                if(friendlist != null){
                    listJ.removeAll(friendlist);
                }
                listJ.remove(host.getId());
            }
            
        } catch (TimeoutException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch (IndexOutOfBoundsException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch(Exception e){
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        
        if(listJ!=null){
            add(hostFriend,listJ);
            GuideLogger.printLog("Tech Friends:"+listJ.size()+" - host:"+host.getId());
        }
        
    }
    
    /**
     * 初中推人
     * @param count
     * @param host
     * @param hostFriend
     */
    private void getJuniorSchoolResults(int count, User host, List<Integer>hostFriend){
        List<Integer> listJ = null;
        listJ = GuideXFiveIDsPreviewLogic.getInstance().getJuniorResults(count, host);
        
        try {
            if(listJ != null){
                List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
                if(friendlist != null){
                    listJ.removeAll(friendlist);
                }
                listJ.remove(host.getId());
            }
            
        } catch (TimeoutException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch (IndexOutOfBoundsException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch(Exception e){
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        
        if(listJ!=null){
            add(hostFriend,listJ);
            GuideLogger.printLog("JuniorSchoolAndElementary Junior Friends:"+listJ.size()+" - host:"+host.getId());
        }
        
    }
    
    /**
     * 小学推人
     * @param count
     * @param host
     * @param hostFriend
     */
    private void getElementaryResults(int count, User host, List<Integer>hostFriend){
        List<Integer> listE = null;        
        listE = GuideXFiveIDsPreviewLogic.getInstance().getElementaryResults(count, host);
        
        try {
            if(listE != null){
                List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
                if(friendlist != null){
                    listE.removeAll(friendlist);
                }
                listE.remove(host.getId());
            }
            
        } catch (TimeoutException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch (IndexOutOfBoundsException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch(Exception e){
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        
        if(listE!=null){
            add(hostFriend,listE);
            GuideLogger.printLog("JuniorSchoolAndElementary Elementary Friends:"+listE.size()+" - host:"+host.getId());
        }
    }
    
    /**
     * 白领推人
     * @param count
     * @param host
     * @param hostFriend
     */
    private void getWorkResults(int count, User host, List<Integer>hostFriend){
        List<Integer> listW = null;
        //bailing
        listW = GuideXFiveIDsPreviewLogic.getInstance().getWorkResults(count, host);
        
        try {
            if(listW != null){
                List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
                if(friendlist != null){
                    listW.removeAll(friendlist);
                }
                listW.remove(host.getId());
            }
            
        } catch (TimeoutException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch (IndexOutOfBoundsException e) {
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
        } catch(Exception e){
            GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        
        if(listW!=null){
            add(hostFriend,listW);
            GuideLogger.printLog("Work Friends:"+listW.size()+" - host:"+host.getId());
        }        
    }
    
    /**
     * 相同IP的推人，做判断 没有数据，推地域
     * @param count
     * @param host
     * @param hostFriend
     * @param req
     */
    private void getSameIpResults(int count, User host, List<Integer>hostFriend, HttpServletRequest req){
        List<Integer> listSameIp = null, listR = null;
        //同IP
        listSameIp = GuideXFiveIDsPreviewLogic.getInstance().getSameIpResults(count, host, req);
        if(listSameIp == null || listSameIp.size() ==0){
            //年龄search2的接口
            listR = GuideXFiveIDsPreviewLogic.getInstance().getRegionResults(count, host);
            if(listR!=null){
                add(hostFriend,listR);
                GuideLogger.printLog("Region Friends:"+listR.size()+" - host:"+host.getId());
            }
        }else{
            try {
                if(listSameIp != null){
                    List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
                    if(friendlist != null){
                        listSameIp.removeAll(friendlist);
                    }
                    listSameIp.remove(host.getId());
                }
                
            } catch (TimeoutException e) {
                GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            } catch (IndexOutOfBoundsException e) {
                GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            } catch(Exception e){
                GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
                e.printStackTrace();
            }
            add(hostFriend,listSameIp);
            GuideLogger.printLog("SameIP Friends:"+listSameIp.size()+" - host:"+host.getId());
        }
    }
    
    /**
     * 相同IP的推人，不做判断
     * @param count
     * @param host
     * @param hostFriend
     * @param req
     */
    private void getSameIpPureResults(int count, User host, List<Integer>hostFriend, HttpServletRequest req){
        List<Integer> listSameIp = null;
        //同IP
        listSameIp = GuideXFiveIDsPreviewLogic.getInstance().getSameIpResults(count, host, req);
        if(listSameIp != null){
            add(hostFriend,listSameIp);
            GuideLogger.printLog("SameIP Friends:"+listSameIp.size()+" - host:"+host.getId());
        }
    }
    
    /**
     * 其他推人
     * @param count
     * @param host
     * @param hostFriend
     */
    private void getRegionResult(int count, User host, List<Integer>hostFriend){
        List<Integer> listR = null;
        //年龄search2的接口
        listR = GuideXFiveIDsPreviewLogic.getInstance().getRegionResults(count, host);
        if(listR!=null){
            add(hostFriend,listR);
            GuideLogger.printLog("Region Friends:"+listR.size()+" - host:"+host.getId());
        }
    }
    
    /**
     * all methods
     * @param count
     * @param host
     * @param hostFriend
     */
    @SuppressWarnings("unused")
    private void getSearchData(int count, User host, List<Integer>hostFriend, HttpServletRequest req){
        List<Integer> listH=null, listC=null, listJ=null, listU=null, listW=null, listR=null,listSameIp=null;
        //high school
        listH = GuideXFiveIDsPreviewLogic.getInstance().getHighschoolResults(count, host);
        if(listH!=null){
            add(hostFriend,listH);
            GuideLogger.printLog("Highschoool Friends:"+listH.size()+" - host:"+host.getId());
        }
        
        //college
        listC = GuideXFiveIDsPreviewLogic.getInstance().getCollegeResults(count, host);
        if(listC!=null){
            add(hostFriend,listC);
            GuideLogger.printLog("College Friends:"+listC.size()+" - host:"+host.getId());
        }
        //junior
        listJ = GuideXFiveIDsPreviewLogic.getInstance().getJuniorResults(count, host);
        
        if(listJ!=null){
            add(hostFriend,listJ);
            GuideLogger.printLog("Junior Friends:"+listJ.size()+" - host:"+host.getId());
        }
        //university
        listU = GuideXFiveIDsPreviewLogic.getInstance().getUniversityResults(count, host);
        
        if(listU!=null){
            add(hostFriend,listU);
            GuideLogger.printLog("Univeristy Friends:"+listU.size()+" - host:"+host.getId());
        }
        //work
        listW = GuideXFiveIDsPreviewLogic.getInstance().getWorkResults(count, host);
        
        if(listW!=null){
            add(hostFriend,listW);
            GuideLogger.printLog("Work Friends:"+listW.size()+" - host:"+host.getId());
        }
        //region
        listR = GuideXFiveIDsPreviewLogic.getInstance().getRegionResults(count, host);        
        if(listR!=null){
            add(hostFriend,listR);
            GuideLogger.printLog("Region Friends:"+listR.size()+" - host:"+host.getId());
        }
        //same ip
        listSameIp = GuideXFiveIDsPreviewLogic.getInstance().getSameIpResults(count, host, req);
        if(listSameIp != null){
            add(hostFriend,listSameIp);
            GuideLogger.printLog("SameIP Friends:"+listSameIp.size()+" - host:"+host.getId());
        }
        
    }
    
    protected IPinyinFilter getPinyinFilter() {
        return new FriendsPinyinFilter();
    }
    
    private void add(List<Integer> toList,List<Integer> fromList){
    	GuideTimeCost cost = GuideTimeCost.begin();
        if (toList == null) return;
        if(toList.size() >= Max_Friend_Count_N) return;
        try {
            synchronized (toList) {
                toList.removeAll(fromList);
                toList.addAll(fromList);
            }
        } catch (Exception e) {
            GuideLogger.writeLine(this,"add() - "+e.toString(),1);
        }
        cost.endFinally();
    }
    
    private static void inc(){
        int getc = Thread_Count.incrementAndGet();
        GuideLogger.printLog("INC Thread_COUNT: "+ getc,GuideLogger.DEBUG);
        
    }
    private static void dec(){
        int getc = Thread_Count.decrementAndGet();
        GuideLogger.printLog("DEC Thread_COUNT: "+ getc,GuideLogger.DEBUG);
        
    }
    public static void main(String []args){
        System.out.println("weige".toUpperCase().charAt(0));
        
        List<Integer> alist = new ArrayList<Integer>();
        for(int i = 0;i<=30;i++){
        	alist.add(i);
        }
        List<Integer> blist = new ArrayList<Integer>();
        for(int i = 5;i<=10;i++){
        	blist.add(i);
        }
        
        alist.removeAll(blist);
        for (Integer i : alist) {
			System.out.println(i);
		}
        
    }

}

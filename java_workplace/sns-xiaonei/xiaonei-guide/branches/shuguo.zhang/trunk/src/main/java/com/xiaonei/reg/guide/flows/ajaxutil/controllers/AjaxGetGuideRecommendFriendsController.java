package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicInteger;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.dodoyo.friends.relation.factory.RelationModelFactory;
import com.dodoyo.friends.relation.logicmodel.IRelationModel;
import com.dodoyo.friends.relationfriend.logic.IRelation;
import com.dodoyo.friends.relationfriend.logic.IRelationFriend;
import com.dodoyo.friends.utils.FriendsUtil;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.friends.selector.filter.FriendsPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.IPinyinFilter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.logic.preview.CommonPreviewLogic;
import com.xiaonei.reg.guide.logic.preview.IDsPreviewLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.guide.view.NewGuideView;

/**
 * AjaxGetGuideRecommendFriendsController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-25 添加说明
 */
@LoginRequired
@Path({"pv-090729-ff.do","ajaxGetXFGuideRecommendFriends.do"})
public class AjaxGetGuideRecommendFriendsController extends AbstractCharacterSelectorController{

	private static final int Max_N = 45; 
	private static final int Max_Thread_N = 88;
	private static final int Redo_Times = 17;
	private static AtomicInteger Thread_COUNT = new AtomicInteger(0);
	
	private static ExecutorService exetorspool = Executors.newCachedThreadPool();
	
	@Get
	@Post
    public String get(Invocation inv) {
        try {
            super.get(inv);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
	
	@SuppressWarnings("unused")
	private NewGuideView getHSGuideView(User host, int count) {
		return CommonPreviewLogic.getInstance().getHSGuideView(host, count);
	}
	
	private void add(List<Integer> toList,List<Integer> fromList){
		GuideTimeCost cost = GuideTimeCost.begin();
		if (toList == null) return;
		if(toList.size() >= Max_N) return;
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
	
	private void addUniversity(List<Integer> toList,int count, User host){
		GuideLogger.printLog("start"+" - host:"+host.getId());
		GuideTimeCost cost = GuideTimeCost.begin();
		if(toList == null) return;
		if(toList.size() >= Max_N) return;
		
		List<Integer> list = IDsPreviewLogic.getInstance().getUniversityResults(count, host);
		cost.endStep();
		if(list!=null){
			add(toList,list);
			GuideLogger.printLog("Univeristy Friends:"+list.size()+" - host:"+host.getId());
		}
		else{
			GuideLogger.printLog("Univeristy Friends null"+" - host:"+host.getId());
		}
		GuideLogger.printLog("addUniversity() end"+" - host:"+host.getId());
		cost.endFinally();
	}
	
	private void addHighschool(List<Integer> toList,int count, User host){
		GuideLogger.printLog("start"+" - host:"+host.getId());
		GuideTimeCost cost = GuideTimeCost.begin();
		if(toList == null) return;
		if(toList.size() >= Max_N) return;
		
		List<Integer> list = IDsPreviewLogic.getInstance().getHighschoolResults(count, host);
		cost.endStep();
		if(list!=null){
			add(toList,list);
			GuideLogger.printLog("Highschoool Friends:"+list.size()+" - host:"+host.getId());
		}
		else{
			GuideLogger.printLog("Highschoool Friends null"+" - host:"+host.getId());
		}
		GuideLogger.printLog("end"+" - host:"+host.getId());
		cost.endFinally();

	}
	
	private void addWork(List<Integer> toList,int count, User host){
		GuideLogger.printLog("addWork() start"+" - host:"+host.getId());
		GuideTimeCost cost = GuideTimeCost.begin();
		if(toList == null) return;
		if(toList.size() >= Max_N) return;
		
		List<Integer> list = IDsPreviewLogic.getInstance().getWorkResults(count, host);
		cost.endStep();
		if(list!=null){
			add(toList,list);
			GuideLogger.printLog("Work Friends:"+list.size()+" - host:"+host.getId());
		}
		else{
			GuideLogger.printLog("Work Friends null"+" - host:"+host.getId());
		}
		GuideLogger.printLog("end"+" - host:"+host.getId());
		cost.endFinally();
	}
	
	private void addJunior(List<Integer> toList,int count, User host){
		GuideLogger.printLog("start"+" - host:"+host.getId());
		GuideTimeCost cost = GuideTimeCost.begin();
		if(toList == null) return;
		if(toList.size() >= Max_N) return;
		List<Integer> list = IDsPreviewLogic.getInstance().getJuniorResults(count, host);
		cost.endStep();
		if(list!=null){
			add(toList,list);
			GuideLogger.printLog("Junior Friends:"+list.size()+" - host:"+host.getId());
		}
		else{
			GuideLogger.printLog("Junior Friends null"+" - host:"+host.getId());
		}
		GuideLogger.printLog("end"+" - host:"+host.getId());
		cost.endFinally();

	}
	
	private void addCollege(List<Integer> toList,int count, User host){
		GuideLogger.printLog("start"+" - host:"+host.getId());
		GuideTimeCost cost = GuideTimeCost.begin();
		if(toList == null) return;
		if(toList.size() >= Max_N) return;
		
		List<Integer> list = IDsPreviewLogic.getInstance().getCollegeResults(count, host);
		cost.endStep();
		if(list!=null){
			add(toList,list);
			GuideLogger.printLog("College Friends:"+list.size()+" - host:"+host.getId());
		}
		else{
			GuideLogger.printLog("College Friends null"+" - host:"+host.getId());
		}
		GuideLogger.printLog("end"+" - host:"+host.getId());
		cost.endFinally();
	}
	
	private void addRegion(List<Integer> toList, int count, User host){
		GuideLogger.printLog("start"+" - host:"+host.getId());
		GuideTimeCost cost = GuideTimeCost.begin();
		if(toList == null) return;
		if(toList.size() >= Max_N) return;
		List<Integer> list = IDsPreviewLogic.getInstance().getRegionResults(count, host);
		cost.endStep();
		if(list!=null){
			add(toList,list);
			GuideLogger.printLog("Region Friends:"+list.size()+" - host:"+host.getId());
		}
		else{
			GuideLogger.printLog("Region Friends null"+" - host:"+host.getId());
		}
		GuideLogger.printLog("end"+" - host:"+host.getId());
		cost.endFinally();
	}
	
	@Override
	protected List<Integer> getCandidate(User host, Map<String, String> params,
			HttpServletRequest request) {
		if(MyStringUtils.equals("stat", request.getParameter("f"))){
			GuideLogger.printLog(" host:"+host.getId());
			return new ArrayList<Integer>();
		}
		else{
			return getCandidate(host,params);
		}
	}
	private static ExecutorService es = Executors.newCachedThreadPool();
	@Override
	protected List<Integer> getCandidate(final User host, Map<String, String> arg1) {
		GuideTimeCost cost = GuideTimeCost.begin();
		//List<Integer> list = GuideXFiveIDsPreviewLogic.getInstance().getProfileResults(88, host, -1);
		//2010-4-19 17:36:06 改成下面这个试试 ？
		long begintime = System.currentTimeMillis();
		Future<List<Integer>> resultset = es.submit(new DoSearchCallable(host));
		List<Integer> list = new ArrayList<Integer>();
		try {
			list = resultset.get(200, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			GuideLogger.printLog(""+" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			//e.printStackTrace();
		} catch (ExecutionException e) {
			GuideLogger.printLog(""+" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			//e.printStackTrace();
		} catch (TimeoutException e) {
			GuideLogger.printLog(""+" host:"+host.getId()+""+" e:"+e.toString(),GuideLogger.ERROR);
			//e.printStackTrace();
    		//System.err.println(" host:"+host.getId()+""+" e:"+e.toString());
		} catch(Exception e){
			GuideLogger.printLog(""+" host:"+host.getId()+""+" e:"+e.toString(),GuideLogger.ERROR);
		}
        long endtime = System.currentTimeMillis();
		GuideLogger.printLog(" host:"+host.getId()+" cost:"+(endtime-begintime)+" get:"+list.size());
		cost.endStep();
		cost.endFinally();
		try {
            list.remove(host.getId());
        } catch (Exception e) {
           GuideLogger.printLog(" e:"+e.toString()+GuideLogger.ERROR);
        }
		return list;
	}
	
	private class DoSearchCallable implements Callable<List<Integer>>{
		User host;
		public DoSearchCallable(User host) {
			this.host = host;
		}
		@Override
		public List<Integer> call() throws Exception {
			return doSearch(host);
		}
		
	}

	private List<Integer> doSearch(final User host) {
		List<Integer> list = new ArrayList<Integer>();
		try {
            int amount = 49;
            List<IRelationModel> relationlist = new ArrayList<IRelationModel>();
            //获取各个阶段的Model
            IRelationModel workerModel = RelationModelFactory.create(host, amount, RelationModelFactory.COWORKER_RELATION_MODEL);
            IRelationModel universityModel = RelationModelFactory.create(host, amount, RelationModelFactory.UNIVERSITY_RELATION_MODEL);
            IRelationModel collegeModel = RelationModelFactory.create(host, amount, RelationModelFactory.COLLEGE_RELATION_MODEL);
            IRelationModel highModel = RelationModelFactory.create(host, amount, RelationModelFactory.HIGH_RELATION_MODEL);
            IRelationModel juniorModel = RelationModelFactory.create(host, amount, RelationModelFactory.JUNIOR_RELATION_MODEL);
            IRelationModel elementaryModel = RelationModelFactory.create(host, amount, RelationModelFactory.ELEMENTARY_RELATION_MODEL);
            //IRelationModel locationModel = RelationModelFactory.create(host, amount, RelationModelFactory.LOCATION_RELATION_MODEL);
            relationlist.add(workerModel);
            relationlist.add(universityModel);
            relationlist.add(collegeModel);
            relationlist.add(highModel);
            relationlist.add(juniorModel);
            relationlist.add(elementaryModel);
            //relationlist.add(locationModel);
            List<IRelation> mayFriends = FriendsUtil.findPeople(host, relationlist);
            
            if(mayFriends != null){
            	int count = 0 ;
            	for (IRelation relation : mayFriends) {
            		if(relation != null){
            			List<IRelationFriend> innerlist = relation.getList();
            			for (IRelationFriend relationFriend : innerlist) {
            				if( count ++ > amount ) break; 
            				list.add(relationFriend.getId());
            			}
            		}
            	}
            }
            
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
	}

	@SuppressWarnings("unused")
	private List<Integer> getCandidate_old(final User host, Map<String, String> arg1) {
		GuideTimeCost cost = GuideTimeCost.begin();
		GuideLogger.printLog("hostid:"+ host.getId()+ " - start");
		final List<Integer> hostFriend = new ArrayList<Integer>();
		cost.endStep();
		Random rand = new Random();
		final int count = rand.nextInt(Max_N)+20;
		cost.endStep();
		
		int stage = 95;//UserAdapter.get(host).getStage();
		GuideLogger.printLog("stage is:"+stage);
		
		
		switch(stage){
		case 10: //Zhongxue
			GuideLogger.printLog("stage is:"+stage);
			addHighschool(hostFriend, count, host);
			addCollege(hostFriend, count, host);
			addJunior(hostFriend, count, host);
			addUniversity(hostFriend, count, host);
			addWork(hostFriend, count, host);
			addRegion(hostFriend, count, host);
			break;
		case 20: // Daxue
			GuideLogger.printLog("stage is:"+stage);
			addUniversity(hostFriend, count, host);
			addHighschool(hostFriend, count, host);
			addJunior(hostFriend, count, host);
			addWork(hostFriend, count, host);
			addCollege(hostFriend, count, host);
			addRegion(hostFriend, count, host);
			break;
		case 30: // Bailing
			GuideLogger.printLog("stage is:"+stage);
			addWork(hostFriend, count, host);
			addUniversity(hostFriend, count, host);
			addHighschool(hostFriend, count, host);
			addCollege(hostFriend, count, host);
			addJunior(hostFriend, count, host);
			addRegion(hostFriend, count, host);
			break;
		case 90:
			GuideLogger.printLog("stage is:"+stage);
			addUniversity(hostFriend, count, host);
			addWork(hostFriend, count, host);
			addHighschool(hostFriend, count, host);
			addJunior(hostFriend, count, host);
			addCollege(hostFriend, count, host);
			addRegion(hostFriend, count, host);
			break;
		default:
			GuideLogger.printLog("stage is:"+stage);
			/*addUniversity(hostFriend, count, host);
			addWork(hostFriend, count, host);			
			addHighschool(hostFriend, count, host);
			addJunior(hostFriend, count, host);
			addCollege(hostFriend, count, host);
			addRegion(hostFriend, count, host);*/
		
		GuideLogger.printLog("Thread_COUNT = "+ Thread_COUNT);
//		synchronized (Thread_COUNT) {
			
			if(Thread_COUNT.get() < Max_Thread_N){
				inc();
				inc();
				inc();
				inc();
				inc();
				inc();
				Thread addtask6 = new Thread(){
					
					@Override
					public void run() {
//						inc();
						try {
							addUniversity(hostFriend, count, host);
						} catch (Exception e) {
							GuideLogger.printLog("addUniversity host:"+host.getId(),GuideLogger.ERROR);
						}finally {
							dec();
						}
					}
					
				};
				Thread addtask1 = new Thread(){
					
					@Override
					public void run() {
//						inc();
						try {
							addWork(hostFriend, count, host);
						} catch (Exception e) {
							GuideLogger.printLog("addWork host:"+host.getId(),GuideLogger.ERROR);
						}finally {
							dec();
						}
					}
					
				};
				Thread addtask2 = new Thread(){
					
					@Override
					public void run() {
//						inc();
						try {
							addHighschool(hostFriend, count, host);
							
						} catch (Exception e) {
							GuideLogger.printLog("addHighschool host:"+host.getId(),GuideLogger.ERROR);
						}finally {
							dec();
						}
					}
					
				};
				Thread addtask3 = new Thread(){
					
					@Override
					public void run() {
//						inc();
						try {
							addJunior(hostFriend, count, host);
							
						} catch (Exception e) {
							GuideLogger.printLog("addJunior host:"+host.getId(),GuideLogger.ERROR);
						}finally {
							dec();
						}
					}
					
				};
				Thread addtask4 = new Thread(){
					
					@Override
					public void run() {
//						inc();
						try {
							addCollege(hostFriend, count, host);
							
						} catch (Exception e) {
							GuideLogger.printLog("addCollege host:"+host.getId(),GuideLogger.ERROR);
						} finally {
							dec();
						}
					}
					
				};
				Thread addtask5 = new Thread(){
					
					@Override
					public void run() {
//						inc();
						try {
							addRegion(hostFriend, count, host);
							
						} catch (Exception e) {
							GuideLogger.printLog("addRegion host:"+host.getId(),GuideLogger.ERROR);
						} finally {
							dec();
						}
					}
					
				};
				
				exetorspool.execute(addtask6);
				exetorspool.execute(addtask1);
				exetorspool.execute(addtask2);
				exetorspool.execute(addtask3);
				exetorspool.execute(addtask4);
				exetorspool.execute(addtask5);
			}
			else{
				GuideLogger.printLog("Thread_COUNT full!"+Thread_COUNT,GuideLogger.WARN);
			}
//		}
			
			
			cost.endStep();
			int i = 0;
			while (++i < Redo_Times) {
				if (hostFriend.size() < Max_N) {
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				} else {
					GuideLogger.printLog("i=" + i + " return " + hostFriend.size() + " Thread C:"+Thread_COUNT);
					cost.endStep();
					cost.endFinally();
					return hostFriend;
				}
			}

			cost.endStep();
			break;
			
		}
		cost.endStep();
		
		
		if(hostFriend!=null){
			GuideLogger.writeLine(this , "getCandidate() - host:"+ host.getId()+" get count:"+hostFriend.size()+ " - end return"+ " Thread C:"+Thread_COUNT);
		}
		else{
			GuideLogger.printLog("getCandidate error! hostFriend is null - return host:"+host.getId()+ " Thread C:"+Thread_COUNT);
		}
		cost.endStep();
		cost.endFinally();
		return hostFriend;
	}

	private static void inc(){
		int getc = Thread_COUNT.incrementAndGet();
		GuideLogger.printLog("INC Thread_COUNT: "+ getc,GuideLogger.DEBUG);
		
	}
	private static void dec(){
		int getc = Thread_COUNT.decrementAndGet();
		GuideLogger.printLog("DEC Thread_COUNT: "+ getc,GuideLogger.DEBUG);
		
	}

	@Override
	protected IPinyinFilter getPinyinFilter() {
		return new FriendsPinyinFilter();
	}

}

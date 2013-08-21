package com.xiaonei.reg.guide.action.a090729;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.component.friends.selector.filter.FriendsPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.IPinyinFilter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.logic.preview.CommonPreviewLogic;
import com.xiaonei.reg.guide.logic.preview.IDsPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.view.NewGuideView;

public class AjaxRecommendFriendsSelectAction extends GuideCharacterSelector{

	private static final int Max_N = 45; 
	private static final int Max_Thread_N = 88;
	private static final int Redo_Times = 17;
	private static AtomicInteger Thread_COUNT = new AtomicInteger(0);
	
	private static ExecutorService exetorspool = Executors.newCachedThreadPool();
	
	@SuppressWarnings("unused")
	private NewGuideView getHSGuideView(User host, int count) {
		return CommonPreviewLogic.getInstance().getHSGuideView(host, count);
	}
	
	private void add(List<Integer> toList,List<Integer> fromList){
		MyTimeCost cost = MyTimeCost.begin();
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
		MyTimeCost cost = MyTimeCost.begin();
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
		MyTimeCost cost = MyTimeCost.begin();
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
		MyTimeCost cost = MyTimeCost.begin();
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
		MyTimeCost cost = MyTimeCost.begin();
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
		MyTimeCost cost = MyTimeCost.begin();
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
		MyTimeCost cost = MyTimeCost.begin();
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
	
	private void addRegion(List<Integer> toList, int count, User host, RegionInfo info){
		GuideLogger.printLog("start"+" - host:"+host.getId());
		MyTimeCost cost = MyTimeCost.begin();
		if(toList == null) return;
		if(toList.size() >= Max_N) return;
		List<Integer> list = IDsPreviewLogic.getInstance().getRegionResults(count, host, info);
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
	protected List<Integer> getCandidate(final User host, Map<String, String> arg1) {
		GuideLogger.printLog("WHY? here getCandidate...",GuideLogger.ERROR);
		MyTimeCost cost = MyTimeCost.begin();
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
		
		
		{
			GuideLogger.writeLine(this , "getCandidate() - host:"+ host.getId()+" get count:"+hostFriend.size()+ " - end return"+ " Thread C:"+Thread_COUNT);
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

	@Override
	protected List<Integer> getCandidateWithRegion(final User host,
			Map<String, String> callerParam, final RegionInfo info) {
		MyTimeCost cost = MyTimeCost.begin();
		final List<Integer> hostFriend = new ArrayList<Integer>();
		Random rand = new Random();
		final int count = rand.nextInt(Max_N)+20;
		cost.endStep();
		
		if(Thread_COUNT.get() < Max_Thread_N){
			inc();
			Thread addtask5 = new Thread(){
				
				@Override
				public void run() {
					try {
						addRegion(hostFriend, count, host, info);
						
					} catch (Exception e) {
						GuideLogger.printLog("addRegion host:"+host.getId(),GuideLogger.ERROR);
					} finally {
						dec();
					}
				}
				
			};
			
			exetorspool.execute(addtask5);
		}
		else{
			GuideLogger.printLog("Thread_COUNT full!"+Thread_COUNT,GuideLogger.WARN);
		}
		
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
		
		{
			GuideLogger.writeLine(this , " - host:"+ host.getId()+" get count:"+hostFriend.size()+ " - end return"+ " Thread C:"+Thread_COUNT);
		}
		
		cost.endStep();
		cost.endFinally();
		return hostFriend;
	}


}

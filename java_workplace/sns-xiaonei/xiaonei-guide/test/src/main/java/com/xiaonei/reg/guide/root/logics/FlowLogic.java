package com.xiaonei.reg.guide.root.logics;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.configs.MappingConfig;
import com.xiaonei.reg.guide.root.configs.StepStatusDefinition;
import com.xiaonei.reg.guide.root.daos.RegFromSourceDAO;
import com.xiaonei.reg.guide.root.daos.StepStatusDAO;
import com.xiaonei.reg.guide.root.models.Flow;
import com.xiaonei.reg.guide.root.models.Module;
import com.xiaonei.reg.guide.root.models.Unit;

public class FlowLogic {
	private static FlowLogic instance = new FlowLogic();

    public static FlowLogic getInstance() {
        return instance;
    }

    private FlowLogic() {
        super();
    }
    
    @SuppressWarnings("unused")
	private Module getPrevModule(Flow flow, int stepstatus){
    	List<Module> list = getDoneSteps(flow, stepstatus);
    	if(list == null){
    		//表示
    	}
    	int lenth = list.size();
    	if(lenth==0)return null;
    	return list.get(lenth-1);
    } 
    
    public Module getNextModule(Flow flow, int stepstatus){
    	assert(flow!=null);
    	List<Module> list = getNeedSteps(flow, stepstatus);
    	if(list == null){
    		//TODO log
    		//return FlowConfig.getDefaultFlow().get;
    		return null;
    	}
    	//if(list.size()==0) return DefaultConfig.getDefaultModule();
    	return list.get(0);
    }
    
    private List<Module> getDoneSteps(Flow flow,int stepstatus){
    	List<Module> models = flow.getModules();
    	if(models == null) return null;
    	List<Module> donelist = new ArrayList<Module>();
    	for(Module m : models){
    		int mstep = m.getBinaryFlag();
    		if((mstep & stepstatus) == mstep){
    			donelist.add(m);
    		}
    	}
    	return donelist;
    }
    private List<Module> getNeedSteps(Flow flow,int stepstatus){
    	assert(flow != null);
    	List<Module> models = flow.getModules();
    	if(models == null) return null;
    	List<Module> donelist = new ArrayList<Module>();
    	for(Module m : models){
    		int mstep = m.getBinaryFlag();
    		if((mstep & stepstatus) != mstep){
    			donelist.add(m);
    		}
    	}
    	return donelist;
    }

	public Flow getUserFlow(User host) {
		String fromsource = null;
		try {
			fromsource = RegFromSourceDAO.getInstance().getFromsourceById(""+host.getId());
		} catch (SQLException e) {
			e.printStackTrace();
		}
		Flow flow = MappingConfig.getFlowByFromsource(fromsource);
		return flow;
	}

	public int getUserStep(User host) {
		int step = 0;
		try {
			step = StepStatusDAO.getInstance().getStatusById(host.getId());
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return step;
	}

	/**
	 * TODO endThisStep
	 *
	 * @param host
	 *
	 */
	public void endThisStep(User host) {
		
	}

	public void endTheStep(User host, Unit unit) {
		try {
			StepStatusDAO.getInstance().updateStatus(host.getId(), StepStatusDefinition.getBinaryFlag(unit.getStepflagname()));
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
	}
}

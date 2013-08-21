package com.xiaonei.reg.guide.root.models;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.reg.guide.util.GuideLogger;

public class Flow {
	private String id;
	private String name;

	private final List<Module> modules = new ArrayList<Module>();
	private final Map<String,Module> ids = new HashMap<String,Module>();//<unitid,module>

	public List<Module> getModules() {
		return modules;
	}

	public boolean hasUnit(Unit unit){
		if(ids.containsKey(unit.getId())){
			return true;
		}
		else return false;
	}
	public void add(Module module){
		modules.add(module);
		ids.put(module.getUnitId(),module);
	}
	
	public Module get(int index){
		return modules.get(index);
	}


	public String getId() {
		return id;
	}


	public void setId(String id) {
		this.id = id;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}
	
	public void printAll(){
		GuideLogger.printLog(" [m:"+getId()+" contains "+modules.size()+"]");
		for(Module m : modules ){
			//MyLogger.printLog(" [m:"+m.getUnitId()+""+m.getUrl()+"]");
			m.printAll();
		}
		GuideLogger.printLog(" [m:"+getId()+" end "+"]");
	}
}

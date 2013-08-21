package com.xiaonei.reg.guide.root.models;

import com.xiaonei.reg.guide.util.GuideLogger;

public class Module {
	private Unit unit;
	private boolean autocomplete;
	private boolean available;
	public Module(Unit unit, boolean autocomplete, boolean available) {
		super();
		this.unit = unit;
		this.autocomplete = autocomplete;
		this.available = available;
	}

	public Unit getUnit() {
		return unit;
	}

	public void setUnit(Unit unit) {
		this.unit = unit;
	}

	public boolean isAutocomplete() {
		return autocomplete;
	}

	public void setAutocomplete(boolean autocomplete) {
		this.autocomplete = autocomplete;
	}

	public boolean isAvailable() {
		return available;
	}

	public void setAvailable(boolean available) {
		this.available = available;
	}

	public String getUnitId(){
		return unit.getId();
	}
	
	public int getBinaryFlag() {
		return unit.getBinaryFlag();
	}
	
	public String getUrl(){
		return unit.getUrl();
	}
	
	public void printAll(){
		GuideLogger.printLog("  ("+unit.getId()+":"+unit.getName()+":"+unit.getRemark()+")");
	}
}

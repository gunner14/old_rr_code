package com.renren.fmwidget.model;

public abstract class AbstractSystemDatas {

	private int id;
	private int dataVerNum;
	private Pos pos;

	public abstract int getTypeNum();

	public void setTypeNum(int typeNum) {
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getDataVerNum() {
		return dataVerNum;
	}

	public void setDataVerNum(int dataVerNum) {
		this.dataVerNum = dataVerNum;
	}

	public Pos getPos() {
		return pos;
	}

	public void setPos(Pos pos) {
		this.pos= pos;
	}
}

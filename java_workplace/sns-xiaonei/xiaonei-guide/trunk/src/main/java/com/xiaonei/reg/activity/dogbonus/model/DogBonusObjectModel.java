package com.xiaonei.reg.activity.dogbonus.model;

public class DogBonusObjectModel {
	private int id;
	private int bonus_id;
	private String bonus_name;
	private int bonus_cost;
	private int bonus_rate;
	private int bonus_count;
	private int bonus_max;
	
	public DogBonusObjectModel(int id, int bonusId, String bonusName,
			int bonusCost, int bonusRate, int bonusCount, int bonusMax) {
		super();
		this.id = id;
		bonus_id = bonusId;
		bonus_name = bonusName;
		bonus_cost = bonusCost;
		bonus_rate = bonusRate;
		bonus_count = bonusCount;
		bonus_max = bonusMax;
	}
	public DogBonusObjectModel() {
		// TODO Auto-generated constructor stub
	}
	public int getBonus_cost() {
		return bonus_cost;
	}
	public void setBonus_cost(int bonusCost) {
		bonus_cost = bonusCost;
	}
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public int getBonus_id() {
		return bonus_id;
	}
	public void setBonus_id(int bonusId) {
		bonus_id = bonusId;
	}
	public String getBonus_name() {
		return bonus_name;
	}
	public void setBonus_name(String bonusName) {
		bonus_name = bonusName;
	}
	public int getBonus_rate() {
		return bonus_rate;
	}
	public void setBonus_rate(int bonusRate) {
		bonus_rate = bonusRate;
	}
	public int getBonus_count() {
		return bonus_count;
	}
	public void setBonus_count(int bonusCount) {
		bonus_count = bonusCount;
	}
	public int getBonus_max() {
		return bonus_max;
	}
	public void setBonus_max(int bonusMax) {
		bonus_max = bonusMax;
	}
	
}

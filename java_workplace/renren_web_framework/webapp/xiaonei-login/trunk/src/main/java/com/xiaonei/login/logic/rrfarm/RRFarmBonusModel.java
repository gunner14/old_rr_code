package com.xiaonei.login.logic.rrfarm;

/**
 * RRFarmBonusModel <br>
 * 
 * @author tai.wang@opi-corp.com Mar 11, 2010 - 3:49:39 PM
 */
public class RRFarmBonusModel {

	public enum BonusType {
		item, money, prodect
	}

	public static class Item {

		int itemId;

		String itemName;

		int itemNum;

		public Item(int itemId, String itemName, int itemNum) {
			super();
			this.itemId = itemId;
			this.itemName = itemName;
			this.itemNum = itemNum;
		}

		public int getItemId() {
			return itemId;
		}

		public String getItemName() {
			return itemName;
		}

		public int getItemNum() {
			return itemNum;
		}
	}

	public static class Money {

		int money;

		public Money(int money) {
			this.money = money;
		}

		public int getMoney() {
			return money;
		}
	}

	private Item item = new Item(0, "", 0);

	private Money money = new Money(0);

	private BonusType type;

	public RRFarmBonusModel(BonusType type, int money) {
		super();
		this.type = type;
		this.money = new Money(money);
	}

	public RRFarmBonusModel(BonusType type, int itemId, String itemName,
			int itemNum) {
		super();
		this.type = type;
		this.item = new Item(itemId, itemName, itemNum);
	}

	public Item getItem() {
		return this.item;
	}

	public Money getMoney() {
		return money;
	}

	public BonusType getType() {
		return type;
	}

}

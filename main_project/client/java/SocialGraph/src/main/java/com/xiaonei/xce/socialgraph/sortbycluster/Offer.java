package com.xiaonei.xce.socialgraph.sortbycluster;


public class Offer {
	private int _userId;
	private int _weight;
	private String _type;
	
	private String _searchType;
	
	public static enum OfferType {
//		WORK("SCOM"), UNIV("SUNI"), HIGH("SHIGH"), SECT("SSECT") //
//		, JUNI("SJUN"), ELEM("SELE"), BASE("SHOME");
		WORK("work"), UNIV("univ"), HIGH("high"), SECT("sect") //
		, JUNI("juni"), ELEM("elem"), BASE("base");
		public final String type;
		private OfferType(String type) {
			this.type = type;
		}		
		public String getType() {
			return type;
		}
	}
	
	public void setBase(int userId, int weight, String searchType) {
		_userId = userId;
		_weight = weight;
		setType(searchType);
	}
	
	public void setUserId(int userId) {
		_userId = userId;
	}
	public void setWeight(int weight) {
		_weight = weight;
	}
	public void setType(String searchType) {
		_searchType = searchType;
		_type = "SBC";
		if (isSameWorkFriend()) {
			_type += "_SCOM";
		} 
		if (isSameUnivFriend()) {
			_type += "_SUNI";
		}
		if (isSameHighFriend()) {
			_type += "_SHIGH";
		}
		if (isSameSectFriend()) {
			_type += "_SSECT";
		}
		if (isSameJuniFriend()) {
			_type += "_SJUN";
		}
		if (isSameElemFriend()) {
			_type += "_SELE";
		}
		if (isSameHomeFriend()) {
			_type += "_SHOME";
		}
	}
	
	public int getUserId() {
		return _userId;
	}
	public int getWeight() {
		return _weight;
	}
	public String getType() {
		return _type;
	}
	
	
	public boolean isSameWorkFriend() {
		return _searchType.equals(OfferType.WORK.getType());
	}
	public boolean isSameUnivFriend() {
		return _searchType.equals(OfferType.UNIV.getType());
	}
	public boolean isSameHighFriend() {
		return _searchType.equals(OfferType.HIGH.getType());
	}
	public boolean isSameSectFriend() {
		return _searchType.equals(OfferType.SECT.getType());
	}
	public boolean isSameJuniFriend() {
		return _searchType.equals(OfferType.JUNI.getType());
	}
	public boolean isSameElemFriend() {
		return _searchType.equals(OfferType.ELEM.getType());
	}
	public boolean isSameHomeFriend() {
		return _searchType.equals(OfferType.BASE.getType());
	}
		
}

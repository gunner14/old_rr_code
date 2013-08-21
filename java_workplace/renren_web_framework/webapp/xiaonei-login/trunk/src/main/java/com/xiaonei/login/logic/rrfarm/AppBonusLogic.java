package com.xiaonei.login.logic.rrfarm;

public interface AppBonusLogic {

	static final String rewardHost = "http://farm.renren.com";

	static final String rewardMoneyUrl = rewardHost
			+ "/farm/action/callback,externalRewardAction.php/rewardMoney";

	static final String rewardItemUrl = rewardHost
			+ "/farm/action/callback,externalRewardAction.php/rewardItem";

	static final String rewardProductUrl = rewardHost
			+ "/farm/action/callback,externalRewardAction.php/rewardProduct";

	final String requestKey = "who1syourdad!";

	public void rewardMoney(int userId, int rewardMoney);

	public void rewardItem(int userId, int rewardItemId, int rewardItemNum);

	public void rewardProduct(int userId, int rewardItemId, int rewardItemNum);
}

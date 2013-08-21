/**
 * 
 */
package com.xiaonei.passport.task.service;

import com.xiaonei.passport.model.RRFarmBonusModel;
import com.xiaonei.passport.model.RRFarmBonusModel.BonusType;


/**
 * RRFarmBonusDef <br>
 * 
 * @author tai.wang@opi-corp.com Mar 11, 2010 - 3:52:21 PM
 */
public interface RRFarmBonusDef {

	final RRFarmBonusModel goldCoins50 = new RRFarmBonusModel(BonusType.money,
			50);

	final RRFarmBonusModel oven = new RRFarmBonusModel(BonusType.item, 8023,
			"烤炉", 1);

	final RRFarmBonusModel cherry = new RRFarmBonusModel(BonusType.item, 1202,
			"樱桃树", 1);

	final RRFarmBonusModel turkey = new RRFarmBonusModel(BonusType.item, 1301,
			"火鸡", 1);

	final RRFarmBonusModel roseSeed = new RRFarmBonusModel(BonusType.item,
			1104, "玫瑰花种子", 1);

	final RRFarmBonusModel roseBouquet = new RRFarmBonusModel(BonusType.item,
			8033, "玫瑰花束", 1);

	final RRFarmBonusModel nest = new RRFarmBonusModel(BonusType.item, 8007,
			"小鸟窝", 1);

	final RRFarmBonusModel whiteMarbleFountain = new RRFarmBonusModel(
			BonusType.item, 6013, "白色大理石喷泉", 1);

	final RRFarmBonusModel doubleSwing = new RRFarmBonusModel(BonusType.item,
			8025, "双人秋千", 1);

}

package com.renren.fmwidget.services;

import java.util.List;

import com.renren.fmwidget.enums.Apps;
import com.renren.fmwidget.model.AbstractCustomDatas;
import com.renren.fmwidget.model.AbstractModel;
import com.renren.fmwidget.model.AbstractSystemDatas;
import com.renren.fmwidget.model.Pos;
import com.renren.fmwidget.model.WidgetDictModel;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel;

public interface SetupWidget {

	/**
	 * install<br>
	 * 安装app
	 * 
	 * @param userId
	 * @param app
	 * @return
	 * @throws Exception
	 * 
	 * @author tai.wang@opi-corp.com Sep 3, 2010 - 11:33:53 AM
	 */
	public int install(final int userId, final Apps app) throws Exception;

	/**
	 * selectAll<br>
	 * 
	 * @param userId
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Sep 3, 2010 - 11:34:09 AM
	 */
	public List<AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas>> selectAllByUserId(
			int userId);

	/**
	 * selectAllDict<br>
	 * 
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Sep 3, 2010 - 12:50:50 PM
	 */
	public List<WidgetDictModel> selectAllDict();

	/**
	 * delete<br>
	 * 
	 * @param userId
	 * @param uid
	 * 
	 * @author tai.wang@opi-corp.com Sep 3, 2010 - 11:34:17 AM
	 */
	public void delete(int userId, int uid);

	/**
	 * updateConfig<br>
	 * 
	 * @param userId
	 * @param uid
	 * @param configModel
	 * 
	 * @author tai.wang@opi-corp.com Sep 3, 2010 - 11:34:27 AM
	 */
	public void updateConfig(int userId, int uid,
			final AbstractSystemDatas configModel);

	/**
	 *updateContent<br>
	 * 
	 * @param userId
	 * @param uid
	 * @param contentModel
	 * 
	 * @author tai.wang@opi-corp.com Sep 3, 2010 - 11:34:35 AM
	 */
	public void updateContent(int userId, int uid,
			final AbstractCustomDatas contentModel);

	/**
	 * updatePosition<br>
	 * 
	 * @param userId
	 * @param uid
	 * @param positionModel
	 * 
	 * @author tai.wang@opi-corp.com Sep 16, 2010 - 4:00:21 PM
	 */
	public void updatePosition(int userId, int uid, final Pos positionModel);

	public void update(int userId, int uid, final AbstractModel<?, ?> model);

	public void batchUpdate(final int userId,
			List<FakemacWidgetRecordModel> models);
	
	public List<FakemacWidgetDictModel> getShopApps(final int userId, int offset);

}
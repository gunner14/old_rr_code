package com.renren.fmwidget.services;

import java.util.ArrayList;
import java.util.List;

import org.springframework.stereotype.Service;

import com.renren.fmwidget.enums.Apps;
import com.renren.fmwidget.model.AbstractCustomDatas;
import com.renren.fmwidget.model.AbstractModel;
import com.renren.fmwidget.model.AbstractSystemDatas;
import com.renren.fmwidget.model.Pos;
import com.renren.fmwidget.model.WidgetDictModel;
import com.renren.fmwidget.utils.JSONCover;
import com.xiaonei.wService.fmwidget.adapter.FakemacWidgetAdapter;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel;

@Service
public class SetupWidgetSerivce implements SetupWidget {

	// @Autowired
	// private WidgetDefaultConfigService configService;

	public SetupWidgetSerivce() {
	}

	private FakemacWidgetAdapter fakemac = FakemacWidgetAdapter.getInstance();

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#install(int,
	 * com.renren.fmwidget.enums.Apps)
	 */
	public int install(final int userId, final Apps app) throws Exception {
		AbstractModel<?, ?> appConfig = app.getModel().newInstance();
		// configService.getConfig(app);
		if (null == appConfig) {
			return -1;
		}
		List<AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas>> allApps = selectAllByUserId(userId);
		int counter = 0;
		for (AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas> model : allApps) {
			if (model.getSystemDatas().getTypeNum() == app.getId()) {
				counter++;
				if (app == Apps.clock) {
					if (counter >= 1)
						return -1;
				} else if (app == Apps.memo) {
					if (counter >= 10)
						return -1;
				} else if (app == Apps.weather) {
					if (counter >= 10)
						return -1;
				}
			}
		}
		appConfig.setCustomDatas(app.getCustomDatas().newInstance());
		AbstractSystemDatas newInstance = app.getSystemDatas().newInstance();
		newInstance.setPos(new Pos());
		appConfig.setSystemDatas(newInstance);
		FakemacWidgetRecordModel model = appConfig.toRecord(userId);
		model.setAppId(app.getId());
		return fakemac.insertFakemacWidgetRecordAndGetId(userId, model);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#selectAllByUserId(int)
	 */
	public List<AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas>> selectAllByUserId(
			int userId) {
		List<FakemacWidgetRecordModel> list = fakemac
				.selectFakemacWidgetRecordByUserid(userId);

		List<AbstractModel<?, ?>> modelList = new ArrayList<AbstractModel<?, ?>>(
				list.size());
		for (FakemacWidgetRecordModel recordModel : list) {

			Apps modelApps = getAppsEnum(recordModel.getAppId());
			if (null == modelApps) {
				continue;
			}

			try {
				AbstractModel<? extends AbstractSystemDatas, ? extends AbstractCustomDatas> model = modelApps
						.getModel().newInstance();
				model.setCustomDatas(modelApps.getCustomDatas().newInstance());
				model.setSystemDatas(modelApps.getSystemDatas().newInstance());
				modelList.add(model.cover(recordModel));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return modelList;
	}

	private Apps getAppsEnum(int appId) {
		for (Apps app : Apps.values()) {
			if (appId == app.getId()) {
				return app;
			}
		}
		return null;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#selectAllDict()
	 */
	public List<WidgetDictModel> selectAllDict() {
		List<FakemacWidgetDictModel> datalist = fakemac
				.selectFakemacWidgetDict(0, 0, Integer.MAX_VALUE);
		List<WidgetDictModel> dictList = new ArrayList<WidgetDictModel>(
				datalist.size());
		for (FakemacWidgetDictModel fakemacWidgetDictModel : datalist) {
			WidgetDictModel model = new WidgetDictModel();
			model.setAppId(fakemacWidgetDictModel.getApp_id());
			model.setAppName(fakemacWidgetDictModel.getApp_name());
			model.setAppOwner(fakemacWidgetDictModel.getApp_owner());
			model
					.setAppDescription(fakemacWidgetDictModel
							.getApp_description());
			dictList.add(model);
		}
		return dictList;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#delete(int, int)
	 */
	public void delete(int userId, int uid) {
		fakemac.updateFakemacWidgetRecordSetDelmarkById(userId, uid, true);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#updateConfig(int, int,
	 * com.renren.fmwidget.model.AbstractSystemDatas)
	 */
	public void updateConfig(int userId, int uid,
			final AbstractSystemDatas configModel) {
		String config = JSONCover.fromObject(configModel).toString();
		fakemac.updateFakemacWidgetRecordSetConfigById(userId, uid, config);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#updateContent(int, int,
	 * com.renren.fmwidget.model.AbstractCustomDatas)
	 */
	public void updateContent(int userId, int uid,
			final AbstractCustomDatas contentModel) {
		String content = JSONCover.fromObject(contentModel).toString();
		fakemac.updateFakemacWidgetRecordSetContentById(userId, uid, content);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#updatePosition(int, int,
	 * com.renren.fmwidget.model.Position)
	 */
	public void updatePosition(int userId, int uid, final Pos positionModel) {
		String position = JSONCover.fromObject(positionModel).toString();

		fakemac.updateFakemacWidgetRecordSetPositionById(userId, uid, position);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#update(int, int,
	 * com.renren.fmwidget.model.AbstractModel)
	 */
	public void update(int userId, int uid, final AbstractModel<?, ?> model) {
		// String sModel = JSONCover.fromObject(model).toString();
		fakemac.updateFakemacWidgetRecordById(userId, uid, model
				.toRecord(userId));
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.renren.fmwidget.services.SetupWidget#batchUpdate(int,
	 * java.util.List)
	 */
	public void batchUpdate(final int userId,
			List<FakemacWidgetRecordModel> models) {
		fakemac.updateFakemakWidgetRecordByIdBatch(userId, models);

	}

	@Override
	public List<FakemacWidgetDictModel> getShopApps(int userId, int offset) {
		final int limit = 50;
		return fakemac.selectFakemacWidgetDict(userId, offset * limit + 1 , limit);
	}

}

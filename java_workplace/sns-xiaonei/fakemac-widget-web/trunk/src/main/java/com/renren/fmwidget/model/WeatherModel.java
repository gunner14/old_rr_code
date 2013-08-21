package com.renren.fmwidget.model;

import net.sf.json.JSONObject;

import com.renren.fmwidget.enums.Apps;
import com.renren.fmwidget.utils.JSONCover;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel;

public final class WeatherModel
		extends
		AbstractModel<WeatherModel.WeatherSystemDatas, WeatherModel.WeatherCustomDatas> {

	public static class WeatherSystemDatas extends AbstractSystemDatas {

		@Override
		public int getTypeNum() {
			return Apps.weather.getId();
		}

	}

	public static class WeatherCustomDatas extends AbstractCustomDatas {

		private String content;

		/**
		 * @param content
		 *            the content to set
		 */
		public void setContent(String content) {
			this.content = content;

		}

		/**
		 * @return the content
		 */
		public String getContent() {
			return content;
		}

	}

	@Override
	public WeatherModel cover(FakemacWidgetRecordModel record) {
		this.getSystemDatas().setId(record.getId());
		try {
			this.setSystemDatas(JSONCover.toObject(JSONObject.fromObject(record
					.getConfig()), WeatherSystemDatas.class));
		} catch (Exception e) {
		}
		this.getSystemDatas().setTypeNum(record.getAppId());
		this.getSystemDatas().setId(record.getId());
		try {
			this.getSystemDatas().setPos(
					JSONCover.toObject(JSONObject.fromObject(record
							.getPosition()), Pos.class));
		} catch (Exception e) {
		}
		try {
			this.setCustomDatas(JSONCover.toObject(JSONObject.fromObject(record
					.getContent()), WeatherCustomDatas.class));
		} catch (Exception e) {
		}
		return this;
	}

}

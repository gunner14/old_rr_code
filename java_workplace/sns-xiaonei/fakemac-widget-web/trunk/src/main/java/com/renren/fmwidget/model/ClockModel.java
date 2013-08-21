package com.renren.fmwidget.model;

import net.sf.json.JSONObject;

import com.renren.fmwidget.enums.Apps;
import com.renren.fmwidget.utils.JSONCover;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel;

public final class ClockModel extends
		AbstractModel<ClockModel.ClockSystemDatas, ClockModel.ClockCustomDatas> {

	public static class ClockSystemDatas extends AbstractSystemDatas {

		@Override
		public int getTypeNum() {
			return Apps.clock.getId();
		}

	}

	public static class ClockCustomDatas extends AbstractCustomDatas {

		private String content;

		/**
		 * @param content
		 *            the content to set
		 */
		public void setContent(String text) {
			this.content = text;

		}

		/**
		 * @return the content
		 */
		public String getContent() {
			return content;
		}

	}

	@Override
	public ClockModel cover(FakemacWidgetRecordModel record) {
		this.getSystemDatas().setId(record.getId());
		try {
			this.setSystemDatas(JSONCover.toObject(JSONObject.fromObject(record
					.getConfig()), ClockSystemDatas.class));
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
					.getContent()), ClockCustomDatas.class));
		} catch (Exception e) {
		}
		return this;
	}

}

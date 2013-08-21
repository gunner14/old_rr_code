package com.renren.fmwidget.model;

import org.apache.commons.lang.StringUtils;

import net.sf.json.JSONObject;

import com.renren.fmwidget.enums.Apps;
import com.renren.fmwidget.utils.JSONCover;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel;

public final class StickyModel extends
		AbstractModel<StickyModel.StickySystemDatas, StickyModel.StickyCustomDatas> {

	public static class StickySystemDatas extends AbstractSystemDatas {

		@Override
		public int getTypeNum() {
			return Apps.memo.getId();
		}

	}

	public static class StickyCustomDatas extends AbstractCustomDatas {

		private String content;

		/**
		 * @param content
		 *            the content to set
		 */
		public void setContent(String text) {
			this.content = StringUtils.substring(text, 0, 200);

		}

		/**
		 * @return the content
		 */
		public String getContent() {
			return content;
		}

	}

	@Override
	public StickyModel cover(FakemacWidgetRecordModel record) {
		this.getSystemDatas().setId(record.getId());
		try {
			this.setSystemDatas(JSONCover.toObject(JSONObject.fromObject(record
					.getConfig()), StickySystemDatas.class));
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
					.getContent()), StickyCustomDatas.class));
		} catch (Exception e) {
		}
		return this;
	}

}

package com.renren.fmwidget.model;

import net.sf.json.JSONObject;

import com.renren.fmwidget.utils.JSONCover;
import com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel;

public abstract class AbstractModel<SystemDatas extends AbstractSystemDatas, CustomDatas extends AbstractCustomDatas> {

	public abstract AbstractModel<?, ?> cover(FakemacWidgetRecordModel record);

	public FakemacWidgetRecordModel toRecord(final int userId) {
		FakemacWidgetRecordModel model = new FakemacWidgetRecordModel();
		model.setUserId(userId);
		JSONObject sysDatasObject = JSONCover.fromObject(getSystemDatas());
		model.setAppId(sysDatasObject.getInt("typeNum"));
		sysDatasObject.remove("typeNum");
		model.setId(sysDatasObject.getInt("id"));
		sysDatasObject.remove("id");
		String config = sysDatasObject.toString();
		model.setConfig(config);
		String content = JSONCover.fromObject(getCustomDatas()).toString();
		model.setContent(content);
		try {
			String position = sysDatasObject.get("pos").toString();
			sysDatasObject.remove("pos");
			model.setPosition(position);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return model;
	}

	public AbstractModel<?, ?> cover(JSONObject json) {
		return JSONCover.toObject(json, this.getClass());
	}

	public JSONObject toJson() {
		return JSONCover.fromObject(this);
	}

//	private Position position;

	private SystemDatas systemDatas;

	private CustomDatas customDatas;

//	public Position getPosition() {
//		return position;
//	}
//
//	public void setPosition(Position position) {
//		this.position = position;
//	}

	public SystemDatas getSystemDatas() {
		return systemDatas;
	}

	@SuppressWarnings("unchecked")
	public void setSystemDatas(AbstractSystemDatas systemDatas) {
		this.systemDatas = (SystemDatas) systemDatas;
	}

	public CustomDatas getCustomDatas() {
		return customDatas;
	}

	@SuppressWarnings("unchecked")
	public void setCustomDatas(AbstractCustomDatas customDatas) {
		this.customDatas = (CustomDatas) customDatas;
	}

}

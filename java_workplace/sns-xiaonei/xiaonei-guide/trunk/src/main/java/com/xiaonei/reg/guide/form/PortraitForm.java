package com.xiaonei.reg.guide.form;

import org.apache.struts.action.ActionForm;
import org.apache.struts.upload.FormFile;

import com.xiaonei.reg.common.SerialFormFile;

/**
 * 白领阶段的上传头像页，用于获取url的form
 * 
 * @author dream
 * @date 2008-10-13
 */
public class PortraitForm extends ActionForm {

	private static final long serialVersionUID = 576166145644470666L;

	private SerialFormFile portrait;

	public FormFile getPortrait() {
		return portrait;
	}

	public void setPortrait(SerialFormFile portrait) {
			this.portrait = portrait;
	}
}

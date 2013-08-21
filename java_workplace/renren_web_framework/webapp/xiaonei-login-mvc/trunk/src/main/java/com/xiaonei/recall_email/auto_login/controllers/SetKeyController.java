package com.xiaonei.recall_email.auto_login.controllers;

import java.util.List;

import org.springframework.web.multipart.MultipartFile;

import com.xiaonei.recall_email.auto_login.logic.RecallKey;
import com.xiaonei.recall_email.auto_login.model.RecallEmailAutoLoginModel;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

@Path("setKey")
public class SetKeyController {

	@Get
	public String get(Invocation inv) {
		return "/views/login/recall_email_auto_login_key_bs.jsp";
	}

	@Post
	public String post(Invocation inv, MultipartFile file) {
		// 读文件
		List<RecallEmailAutoLoginModel> list = getIdList(file);
		// 生成链接
		transferKey(list);
		// 保存
		saveKey(list);
		return get(inv);
	}

	private void saveKey(final List<RecallEmailAutoLoginModel> list) {
		RecallKey.getInstance().saveKey(list);
	}

	private void transferKey(final List<RecallEmailAutoLoginModel> list) {
		RecallKey.getInstance().transferKey(list);
	}

	private List<RecallEmailAutoLoginModel> getIdList(MultipartFile file) {
		return RecallKey.getInstance().getIdList(file);
	}
}

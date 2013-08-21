package com.renren.passport.service;

import com.renren.xoa2.annotation.XoaService;

/**
 * 
 * @author xiaodong.yuan
 * @author shuguo.zhang
 * 登陆提供的对外XOA接口
 */
@XoaService("main.passport.xoa.renren.com")
public interface ILoginXoaService extends LoginXoaService.Iface, ILoginService{
	
}

package com.xiaonei.reg.common.constants;

import com.dodoyo.opt.DodoyoResource;

public interface IRegFromSource {

	public static final String PAGEIDS = "pageids";

	/** 4399 广告来源 */
	public static final String S_4399 = DodoyoResource.getString("S_4399", PAGEIDS);

	/** 教育部来源 */
	public static final String S_EDU = DodoyoResource.getString("S_EDU", PAGEIDS);

	/** @deprecated 短表单邀请注册 */
	public static final String S_SREG = DodoyoResource.getString("S_SIREG", PAGEIDS);

	/** 短表单普通注册 */
	public static final String S_SCREG = DodoyoResource.getString("S_SCREG", PAGEIDS);

	/** 短表单邀请注册 */
	public static final String S_SIREG = DodoyoResource.getString("S_SIREG", PAGEIDS);

	/** 高三通道 */
	public static final String S_G3REG = DodoyoResource.getString("S_G3REG", PAGEIDS);

	/** ID注册来的 */
	public static final String S_IDREG = DodoyoResource.getString("S_IDREG", PAGEIDS);

	/** Page注册来的 */
	public static final String S_PAGE = DodoyoResource.getString("S_PAGE", PAGEIDS);
	
	/** 渠道来养狗的 */
	public static final String S_CHANNALDOG = DodoyoResource.getString("S_CHANNALDOG", PAGEIDS);

	/** 明星注册页面 */
	public static final String S_SUPERSTAR = DodoyoResource.getString("S_SUPERSTAR", PAGEIDS);
	
	/** 网吧后缀 */
	public static final String S_SUF_NETBAR = DodoyoResource.getString("S_SUF_NETBAR", PAGEIDS);
	
	/** 麻子面馆 */
    public static final String S_MZMG = DodoyoResource.getString("S_MZMG", PAGEIDS);
    
    /** web game */
    public static final String S_GAME = DodoyoResource.getString("S_GAMEREG", PAGEIDS);
}

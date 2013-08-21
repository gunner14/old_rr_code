package com.xiaonei.platform.core.model;

import java.io.Serializable;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.xce.userbasic.UserBasicInfo;

/**
 * 
 * UserBasic.java
 * @author zhangjie@opi-corp.com
 */
public class UserBasic extends UserBasicInfo implements Serializable {
	private static final long serialVersionUID = 8234650290181069541L;

	public String getHead_Url() {
		return HeadUrlUtil.getHeadFullUrl(headUrl);
	}

	public String getTiny_Url() {
		return HeadUrlUtil.getHeadFullUrl(tinyUrl);
	}

	public UserBasic(User usr) {
		this.id = usr.getId();
		this.name = usr.getName();
		this.status = usr.getStatus();
	}

	public UserBasic(int id) {
		this.id = id;
	}

	public UserBasic() {
	}

	public String getTinyFullUrl() {
		return HeadUrlUtil.getHeadFullUrl(this.tinyUrl);
	}

	public String getUnivName() {
		return NetworkManager.getInstance().getNetById(this.univ).getName();
	}

	public String getDoing2() {
		if (com.xiaonei.platform.core.opt.OpiConstants.isXiaonei()) {
			doing = doing
					.replaceAll(
							"\\((D|d)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/imgpro/icons/doritos_log.jpg' alt='多力多滋' class='status-em' style='width:12px;height:12px' />")
					.replaceAll(
							"\\((R|r)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/imgpro/icons/ico_rocket.gif' alt='神七问天' class='status-em' style='width:12px;height:12px' />")
					.replaceAll(
							"\\((W|w)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/imgpro/icons/ico_spacewalker.gif' alt='我是宇航员' class='status-em' style='width:12px;height:12px' />")
					.replaceAll(
							"\\((F|f)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/img/ems/fist.gif' alt='捍卫校内' class='status-em' style='width:12px;height:12px' />")
					.replaceAll(
							"\\((3)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/imgpro/icons/3years.gif' alt='校内三周年' class='status-em' style='width:12px;height:12px' />")
					.replaceAll(
							"\\((H|h)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/imgpro/icons/philips.jpg' alt='飞利浦乐享健康好生活' class='status-em' style='width:12px;height:12px' />")
					.replaceAll(
							"\\((V|v)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/imgpro/icons/rose.gif' alt='情人节' class='status-em' style='width:12px;height:12px' />");
		} else {
			doing = doing
					.replaceAll(
							"\\((F|f)\\)",
							"<img src='"
									+ OpiConstants.urlStatic
									+ "/img/ems/fist.gif' alt='捍卫开心' class='status-em' style='width:12px;height:12px' />");
		}
		return doing
				.replaceAll(
						"\\((L|l)\\)",
						"<img src='"
								+ OpiConstants.urlStatic
								+ "/img/ems/love.gif' alt='爱' class='status-em' style='width:12px;height:12px' />")
				.replaceAll(
						"\\((P|p)\\)",
						"<img src='"
								+ OpiConstants.urlStatic
								+ "/img/ems/bulb.gif' alt='绿色环保大使' class='status-em' style='width:12px;height:12px' />")
				.replaceAll(
						"\\((C|c)\\)",
						"<img src='"
								+ OpiConstants.urlStatic
								+ "/img/ems/candle.gif' alt='为震区祈福' class='status-em' style='width:12px;height:12px' />")
				.replaceAll(
						"\\((B|b)\\)",
						"<img src='"
								+ OpiConstants.urlStatic
								+ "/img/ems/gsilk.gif' alt='绿丝带' class='status-em' style='width:12px;height:12px' />")
				.replaceAll(
						"\\((T|t)\\)",
						"<img src='"
								+ OpiConstants.urlStatic
								+ "/img/ems/torch.gif' alt='点燃百年奥运的火炬' class='status-em' style='width:12px;height:12px' />")
				.replaceAll(
						"\\((Y|y)\\)",
						"<img src='"
								+ OpiConstants.urlStatic
								+ "/img/ems/yeah.gif' alt='中国必胜' class='status-em' style='width:12px;height:12px' />")
				.replaceAll(
						"\\((G|g)\\)",
						"<img src='"
								+ OpiConstants.urlStatic
								+ "/img/ems/good.gif' alt='中国金牌第一' class='status-em' style='width:12px;height:12px' />")
				.replaceAll("[\\r]|[\\n]", "");
	}
}

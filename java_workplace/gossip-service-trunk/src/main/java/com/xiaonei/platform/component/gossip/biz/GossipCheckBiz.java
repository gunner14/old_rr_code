package com.xiaonei.platform.component.gossip.biz;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.antispam.model.InfoType;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.component.gossip.model.GossipCheckResult;
import com.xiaonei.platform.component.gossip.service.GossipErrors;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.AlbumUtil;
import com.xiaonei.platform.framework.exception.AntispamException;
import com.xiaonei.platform.framework.exception.FormValidationException;

@Service
public class GossipCheckBiz {
	
	private Log logger = LogFactory.getLog(this.getClass());
	
	public GossipCheckResult checkBody(User host,User guest,Gossip gossip) throws FormValidationException, AntispamException {
		StringBuffer body = new StringBuffer().append(gossip.getBody().getBody()).append(gossip.getHeadUrl());
		if (StringUtils.isBlank(body.toString())) {
			throw new FormValidationException(GossipErrors.no_content, "留言不能为空");
		} else if (body.length() > 2000) {
			throw new FormValidationException(GossipErrors.over_length, "留言长度不能超过2000个字符");
		}
		
	
		
		if(logger.isDebugEnabled()){
			logger.debug(String.format("图片地址：%s  ||  %s", gossip.getTinyurl(),gossip.getLarge_url()));
		}
		
		//FOR THIS CODE JUST FOR ANTISPAM PIC WITH GOSSIP
		if(gossip.getHeadUrl()!=null ||gossip.getLarge_url()!=null){
			
			body.append(";").append(AlbumUtil.getPhotoURL(gossip.getHeadUrl()));
			body.append(";").append(AlbumUtil.getPhotoURL(gossip.getLarge_url()));
			
			CheckResult crb = AntiSpamAdapter.getInstance().checkAndFilter(host.getId(), CheckType.GOSSIP_TYPE, body.toString());
			String dealBody = crb.getM_content();
			if (crb.getM_type() == CheckResult.PROHIBITED) {
				throw new AntispamException(GossipErrors.forbidden_content, crb.getShowMessage());
			} else if (crb.getM_type() == CheckResult.SYSTEM_ERROR) {
				throw new AntispamException(GossipErrors.unknonw, "系统错误");
			}
			
		}
		
		CheckResult crb = AntiSpamAdapter.getInstance().checkAndFilter(host.getId(), CheckType.GOSSIP_TYPE, gossip.getBody().getBody());
		String dealBody = crb.getM_content();
		if (crb.getM_type() == CheckResult.PROHIBITED) {
			throw new AntispamException(GossipErrors.forbidden_content, crb.getShowMessage());
		} else if (crb.getM_type() == CheckResult.SYSTEM_ERROR) {
			throw new AntispamException(GossipErrors.unknonw, "系统错误");
		}
		
		int activity = AntiSpamHome.getInstance().checkActivity(host.getId(),guest.getId(), CheckType.GOSSIP_TYPE, dealBody);
		if (activity == CheckResult.ACTIVE_TYPE_FAST
				|| activity == CheckResult.ACTIVE_TYPE_FAST_OVER_COUNT) {
			throw new AntispamException(GossipErrors.fast_over_count, CheckResult.getShowMessage(activity));
		} else if (activity == CheckResult.ACTIVE_TYPE_SAME
				|| activity == CheckResult.ACTIVE_TYPE_SAME_OVER_COUNT) {
			throw new AntispamException(GossipErrors.same_over_count, CheckResult.getShowMessage(activity));
		} else if (activity == CheckResult.SYSTEM_ERROR) {
			logger.error("GossipError(antispam_system_error):"
					+ "hostid:" + host.getId() + " guestid:" + guest.getId()
					+ " body_len:" + body.length());
		}
		gossip.getBody().setBody(dealBody);
		GossipCheckResult result = new GossipCheckResult();
		if(crb!=null && crb.getM_type()==CheckResult.CONTENT_NEED_AUDIT && crb.getInfoType()==InfoType.INFO_POLITICS){
			result.audit = true;
			result.colorBody = crb.getM_info();
		}
		return result;
	}
	
	public String checkBody(int host,int guest,String body) throws FormValidationException, AntispamException {
		if (StringUtils.isBlank(body)) {
			throw new FormValidationException(GossipErrors.no_content, "留言不能为空");
		} else if (body.length() > 2000) {
			throw new FormValidationException(GossipErrors.over_length, "留言长度不能超过2000个字符");
		}
		CheckResult crb = AntiSpamAdapter.getInstance().checkAndFilter(host, CheckType.GOSSIP_TYPE, body);
		body = crb.getM_content();
		if (crb.getM_type() == CheckResult.PROHIBITED) {
			throw new AntispamException(GossipErrors.forbidden_content, crb.getShowMessage());
		} else if (crb.getM_type() == CheckResult.SYSTEM_ERROR) {
			throw new AntispamException(GossipErrors.unknonw, "系统错误");
		}
		int activity = AntiSpamHome.getInstance().checkActivity(host,guest, CheckType.GOSSIP_TYPE, body);
		if (activity == CheckResult.ACTIVE_TYPE_FAST
				|| activity == CheckResult.ACTIVE_TYPE_FAST_OVER_COUNT) {
			throw new AntispamException(GossipErrors.fast_over_count,  CheckResult.getShowMessage(activity));
		} else if (activity == CheckResult.ACTIVE_TYPE_SAME
				|| activity == CheckResult.ACTIVE_TYPE_SAME_OVER_COUNT) {
			throw new AntispamException(GossipErrors.same_over_count,  CheckResult.getShowMessage(activity));
		} else if (activity == CheckResult.SYSTEM_ERROR) {
			System.err.println("GossipError(antispam_system_error):"
					+ "hostid:" + host + " guestid:" + guest
					+ " body_len:" + body.length());
		}
		return body;
	}

}

package com.xiaonei.platform.component.gossip.biz;

import java.util.Date;

import org.springframework.stereotype.Service;

import com.xiaonei.admin.biz.model.audit.AuditPhotoView;
import com.xiaonei.admin.biz.model.audit.AuditReplyView;
import com.xiaonei.admin.biz.model.audit.AuditPhotoView.PhotoBizType;
import com.xiaonei.admin.biz.model.audit.AuditReplyView.AuditReplyType;
import com.xiaonei.admin.common.logic.audit.AuditLogic;
import com.xiaonei.antispam.model.InfoType;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.AlbumUtil;

@Service
public class GossipAuditBiz {
	public void saveAudit(long replyId,long contentId,User host,User guest,User ucc,String colorBody){
		AuditReplyView view = new AuditReplyView();
		view.setReplyId(replyId);
		view.setContentId(contentId);

		view.setCreatorId(host.getId());
		view.setCreatorName(host.getName());
		if(host.getId()!=guest.getId()){
			view.setOwnerId(guest.getId());
			view.setOwnerName(guest.getName());
		}
		else if(ucc!=null){
			view.setOwnerId(ucc.getId());
			view.setOwnerName(ucc.getName());
		}
		view.setBody(colorBody);
		view.setCreateTime(new Date());
		view.setAuditType(InfoType.INFO_POLITICS);
		
		view.setType(AuditReplyType.GOSSIP);
		AuditLogic.getInstance().saveAuditReply(view);
	}
	
	public void saveAuditPic(long gid,int owner,String headUrl){
		//留言板图片审核代码
		AuditPhotoView  audit = new AuditPhotoView();
		audit.setPhotoId(gid);
		audit.setOwnerId(owner);
		audit.setBizType(PhotoBizType.GOSSIP);
		audit.setPhotoUrl(AlbumUtil.getPhotoURL(headUrl));
		AuditLogic.getInstance().saveAuditPhoto(audit);
	}
}

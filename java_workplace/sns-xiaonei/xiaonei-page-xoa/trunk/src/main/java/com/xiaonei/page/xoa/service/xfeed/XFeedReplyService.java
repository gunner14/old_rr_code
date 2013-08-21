package com.xiaonei.page.xoa.service.xfeed;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import xce.feed.FeedReply;

import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.ReplyContainer;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.component.tools.JSONUtil;
import com.xiaonei.platform.component.xfeed.reply.XFeedReplyer;
import com.xiaonei.xce.feed.reply.FeedReplyDispatcher;
import com.xiaonei.xce.feed.reply.FeedReplyObj;

/**
 * @author yi.li@renren-inc.com since 2011-9-30
 * 
 */
@Service
public class XFeedReplyService {

	private final static Log logger = LogFactory.getLog("PAGEXOA");

	/**
	 * 给指定UGC的新鲜事添加回复
	 * 
	 * @param replys
	 *            回复数据
	 * @param source
	 *            UGC源
	 * @param feedType
	 *            新鲜事
	 * @param actor
	 *            回复人
	 * @param toUid
	 *            被回复人id
	 * @param sourceOwnerId
	 *            UGC owner ID
	 * @throws Exception
	 */
	public void addFeedReply(ReplyContainer replys, long source, int feedType,
			int actor, int toUid, int sourceOwnerId) throws Exception {
		if (replys.getCount() < 1) {
			return;
		}
		if (sourceOwnerId <= 0) {	//兼容接口变化，以前没有sourceOwnerId
			sourceOwnerId = actor;
		}
		if (logger.isDebugEnabled()) {
			logger.debug("Add Replys: " + replys + " source: " + source
					+ ", feedType: " + feedType + ", toUid: " + toUid
					+ ", ownerId:" + sourceOwnerId);
		}
		addFeedReplyOldTime(replys, source, feedType, sourceOwnerId);

		// TODO 是否发送评论新鲜事

	}

	/**
	 * 老版新鲜事添加回复
	 * 
	 * @param replys
	 * @param source
	 * @param feedType
	 * @param actor
	 */
	private void addFeedReplyOldTime(ReplyContainer replys, long source,
			int feedType, int actor) {
		String oldReply = XFeedUtil.getFeedReplyXml(replys.getOldReply());
		long oldReplyId = replys.getOldReply() != null ? replys.getOldReply()
				.getReplyId() : 0L;
		String newReply = XFeedUtil.getFeedReplyXml(replys.getNewReply());
		long newReplyId = replys.getNewReply() != null ? replys.getNewReply()
				.getReplyId() : 0L;
		FeedReply feedReply = new FeedReply(oldReplyId, oldReply, newReplyId,
				newReply, replys.getCount());
		XFeedReplyer.getInstance().addFeedReply(source, feedType, actor,
				feedReply);
	}

	/**
	 * 新版新鲜事添加回复
	 * 
	 * @param replys
	 * @param source
	 * @param feedType
	 * @param actor
	 */
	@SuppressWarnings("unused")
	private void addFeedReplyNew(ReplyContainer replys, long source,
			int feedType, int actor) {
		FeedReplyObj oldReplyObj = getFeedReplyObj(replys.getOldReply());
		FeedReplyObj newReplyObj = getFeedReplyObj(replys.getNewReply());
		FeedReplyDispatcher.getInstance().addFeedReply(newReplyObj,
				oldReplyObj, replys.getCount(), source, feedType, actor);
	}

	/**
	 * 删除新鲜事回复
	 * 
	 * @param replys
	 *            删除回复之后保留的新鲜事回复数据
	 * @param replyId
	 *            被删除的回复id
	 * @param source
	 *            回复UGC源id
	 * @param feedType
	 *            新鲜事类型
	 * @param actor
	 *            删除触发人
	 * @param sourceOwnerId
	 *            UGC owner ID
	 * @throws Exception
	 */
	public void removeFeedReply(ReplyContainer replys, long replyId,
			long source, int feedType, int actor, int sourceOwnerId)
			throws Exception {
		if (logger.isDebugEnabled()) {
			logger.debug("Remove Replys: " + replys);
		}
		removeFeedReplyOldTime(replys, replyId, source, feedType, sourceOwnerId);
	}

	private void removeFeedReplyOldTime(ReplyContainer replys, long replyId,
			long source, int feedType, int actor) {
		String oldReply = XFeedUtil.getFeedReplyXml(replys.getOldReply());
		long oldReplyId = replys.getOldReply() != null ? replys.getOldReply()
				.getReplyId() : 0L;
		String newReply = XFeedUtil.getFeedReplyXml(replys.getNewReply());
		long newReplyId = replys.getNewReply() != null ? replys.getNewReply()
				.getReplyId() : 0L;
		FeedReply feedReply = new FeedReply(oldReplyId, oldReply, newReplyId,
				newReply, replys.getCount());
		XFeedReplyer.getInstance().removeFeedReply(source, feedType, actor,
				replyId, feedReply);
	}

	@SuppressWarnings("unused")
	private void removeFeedReplyNew(ReplyContainer replys, long replyId,
			long source, int feedType, int actor) {
		FeedReplyObj oldReplyObj = getFeedReplyObj(replys.getOldReply());
		FeedReplyObj newReplyObj = getFeedReplyObj(replys.getNewReply());
		FeedReplyDispatcher.getInstance().removeFeedReply(newReplyObj,
				oldReplyObj, replys.getCount(), replyId, source, feedType,
				actor);
	}

	private FeedReplyObj getFeedReplyObj(PageFeedReply reply) {
		if (null == reply) {
			return null;
		}
		FeedReplyObj replyObj = new FeedReplyObj();
		replyObj.setFromId((long) reply.getFromId());
		replyObj.setFromName(reply.getFromName());
		// 貌似为了符合JSON规范？前端新鲜事模板不会将body用""括起来
		replyObj.setContent(JSONUtil.quote(reply.getContent()));
		replyObj.setImContent(reply.getImContent());
		replyObj.setReplyType(reply.getReplyType());
		replyObj.setReplyId(reply.getReplyId());
		replyObj.setTime(reply.getTime());
		replyObj.setTinyImg(reply.getTinyImg());
		return replyObj;
	}
}

package com.xiaonei.page.xoa.controllers.xfeed;

import java.util.ArrayList;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.annotation.JsonParam;
import com.xiaonei.album.home.AlbumHome;
import com.xiaonei.album.model.AlbumComment;
import com.xiaonei.blog.home.BlogHome;
import com.xiaonei.blog.model.Entry;
import com.xiaonei.blog.model.EntryComment;
import com.xiaonei.blog.view.CommentListView;
import com.xiaonei.page.remotelog.LogEntry;
import com.xiaonei.page.remotelog.LogEntryUtil;
import com.xiaonei.page.remotelog.LogType;
import com.xiaonei.page.remotelog.RemoteLogService;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.blog.BlogService;
import com.xiaonei.page.service.blog.ForFeedBlogCommentService;
import com.xiaonei.page.service.photo.ForFeedPageAlbumService;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.ReplyContainer;
import com.xiaonei.page.xoa.service.xfeed.FeedTypeMap;
import com.xiaonei.page.xoa.service.xfeed.XFeedReplyService;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * @author yi.li@renren-inc.com since 2011-9-28
 */
@Path("reply")
public class ReplyFeedController {

    private final static Log logger = LogFactory.getLog("PAGEXOA");

    @Autowired
    private XFeedReplyService feedReplyService;

    @Autowired
    private ForFeedBlogCommentService forFeedBlogCommentService;

    @Autowired
    private BlogService blogService;

    @Autowired
    private BlogHome blogHome;

    @Autowired
    private ForFeedPageAlbumService feedAlbumAsyn;

    @Autowired
    private PageService pageService;

    @Autowired
    private AuthService authService;

    @Post("doing/add")
    public Object addDoingComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long doingId,
            @Param("ownerId") int doingOwnerId, @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(doingOwnerId, uid,
                    doingId, "TYPE_XOA_FEED", LogType.STATE_REPLY, null);
            RemoteLogService.getInstance().doLog(log);

            feedReplyService.addFeedReply(replys, doingId,
                    (int) FeedDefUtil.PAGE_STATUS_UPDATE_REPLY, uid, toId,
                    doingOwnerId);

        } catch (Exception e) {
            logger.error("Add doing comment error", e);
        }
        return "@";
    }

    @Post("doing/del")
    public Object delDoingComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long doingId,
            @Param("ownerId") int doingOwnerId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.removeFeedReply(replys, replyId, doingId,
                    (int) FeedDefUtil.PAGE_STATUS_UPDATE_REPLY, uid,
                    doingOwnerId);
        } catch (Exception e) {
            logger.error("Remove doing comment error", e);
        }
        return "@";
    }

    @Post("blog/add")
    public Object addBlogComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long blogId,
            @Param("ownerId") int blogOwnerId, @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(blogOwnerId, uid, blogId,
                    "TYPE_XOA_FEED", LogType.NOTE_COMMIT, null);
            RemoteLogService.getInstance().doLog(log);

            Entry e = blogService.getBlogWithoutLog(blogId, blogOwnerId);
            User user = WUserAdapter.getInstance().get(uid);

            List<EntryComment> cmtList = null;
            List<CommentListView> viewList = null;
            if (e.getCommentCount() > 0) {
                viewList = blogHome.getBlogCommentListWithFilter(user,
                        e.getId(), blogOwnerId, 1, 100, true);
            }
            cmtList = blogHome.commentListViewToEntryComment(viewList);
            if (cmtList == null) {
                cmtList = new ArrayList<EntryComment>();
            }
            forFeedBlogCommentService.sendHomeCommentEntryFeed(user,
                    WUserAdapter.getInstance().get(blogOwnerId), e, replys
                            .getNewReply().getReplyId(), cmtList, false, toId);

            // feedReplyService
            // .addFeedReply(replys, blogId,
            // (int) FeedDefUtil.PAGE_BLOG_PUBLISH, uid, toId,
            // blogOwnerId);
        } catch (Exception e) {
            logger.error("Add blog comment error", e);
        }
        return "@";
    }

    @Post("blog/del")
    public Object delBlogComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long blogId, @Param("ownerId") int blogOwnerId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(blogOwnerId, uid,
                    replyId, "TYPE_XOA_FEED", LogType.NOTE_COMMIT_DEL, null,
                    blogId + "");
            RemoteLogService.getInstance().doLog(log);
            
            Entry e = blogService.getBlogWithoutLog(blogId, blogOwnerId);
            User user = WUserAdapter.getInstance().get(uid);

            List<EntryComment> cmtList = null;
            List<CommentListView> viewList = null;
            if (e.getCommentCount() > 0) {
                viewList = blogHome.getBlogCommentListWithFilter(user,
                        e.getId(), blogOwnerId, 1, 100, true);
            }
            cmtList = blogHome.commentListViewToEntryComment(viewList);
            long delId = replyId;
            if (cmtList == null) {
                cmtList = new ArrayList<EntryComment>();
            } else
                delId = cmtList.get(0).getAuthor();

            forFeedBlogCommentService.sendHomeCommentEntryFeed(user,
                    WUserAdapter.getInstance().get(blogOwnerId), e, delId,
                    cmtList, false, 0);

            // feedReplyService.removeFeedReply(replys, replyId, blogId,
            // (int) FeedDefUtil.PAGE_BLOG_PUBLISH, uid, blogOwnerId);
        } catch (Exception e) {
            logger.error("Remove blog comment error", e);
        }
        return "@";
    }

    @Post("album/add")
    public Object addAlbumComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long albumId,
            @Param("ownerId") int albumOwnerId, @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(albumOwnerId, uid,
                    albumId, "TYPE_XOA_FEED", LogType.ALBUM_COMMIT, null);
            RemoteLogService.getInstance().doLog(log);

            AlbumComment entry = AlbumHome.getInstance().getAlbumComment(
                    replys.getNewReply().getReplyId(), albumOwnerId, albumId);
            User user = WUserAdapter.getInstance().get(uid);
            // feedAlbumAsyn.deleteCommentFeed(albumId, entry, albumOwnerId,
            // user, pageService.get(albumOwnerId),
            // replys.getNewReply().getReplyId(),
            // authService.isPageAdministrator(uid, albumOwnerId));

            List<AlbumComment> cmtList = AlbumHome.getInstance()
                    .getAlbumCommentList(albumId, albumOwnerId, 1, 100, true);
            feedAlbumAsyn.sendHomeCommentEntryFeed(user, pageService
                    .get(albumOwnerId), entry, replys.getNewReply()
                    .getReplyId(), cmtList, false, 0, authService
                    .isPageAdministrator(uid, albumOwnerId));
            // feedReplyService.addFeedReply(replys, albumId,
            // (int) FeedDefUtil.PAGE_PHOTO_PUBLISH, uid, toId,
            // albumOwnerId);
        } catch (Exception e) {
            logger.error("Add album comment error", e);
        }
        return "@";
    }

    @Post("album/del")
    public Object delAlbumComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long albumId,
            @Param("ownerId") int albumOwnerId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(albumOwnerId, uid,
                    replyId, "TYPE_XOA_FEED", LogType.ALBUM_COMMIT_DEL, null,
                    albumId + "");
            RemoteLogService.getInstance().doLog(log);
            AlbumComment entry = AlbumHome.getInstance().getAlbumComment(
                    replyId, albumOwnerId, albumId);
            User user = WUserAdapter.getInstance().get(uid);
            feedAlbumAsyn.deleteCommentFeed(albumId, entry, albumOwnerId, user,
                    pageService.get(albumOwnerId), replyId,
                    authService.isPageAdministrator(uid, albumOwnerId));
        } catch (Exception e) {
            logger.error("Remove album comment error", e);
        }
        return "@";
    }

    @Post("photo/add")
    public Object addPhotoComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long photoId,
            @Param("ownerId") int photoOwnerId, @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(photoOwnerId, uid,
                    photoId, "TYPE_XOA_FEED", LogType.PHOTO_COMMIT, null);
            RemoteLogService.getInstance().doLog(log);
            feedReplyService.addFeedReply(replys, photoId,
                    (int) FeedDefUtil.PAGE_SINGLE_PHOTO, uid, toId,
                    photoOwnerId);
        } catch (Exception e) {
            logger.error("Add photo comment error", e);
        }
        return "@";
    }

    @Post("photo/del")
    public Object delPhotoComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long photoId,
            @Param("ownerId") int photoOwnerId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(photoOwnerId, uid,
                    replyId, "TYPE_XOA_FEED", LogType.PHOTO_COMMIT_DEL, null,
                    photoId + "");
            RemoteLogService.getInstance().doLog(log);
            feedReplyService.removeFeedReply(replys, replyId, photoId,
                    (int) FeedDefUtil.PAGE_SINGLE_PHOTO, uid, photoOwnerId);
        } catch (Exception e) {
            logger.error("Remove photo comment error", e);
        }
        return "@";
    }

    @Post("share/blog/add")
    public Object addShareBlogComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long shareId,
            @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.addFeedReply(replys, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_BLOG, uid, toId, 0);
        } catch (Exception e) {
            logger.error("Add share comment error", e);
        }
        return "@";
    }

    @Post("share/blog/del")
    public Object delShareBlogComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long shareId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.removeFeedReply(replys, replyId, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_BLOG, uid, 0);
        } catch (Exception e) {
            logger.error("Remove share comment error", e);
        }
        return "@";
    }

    @Post("share/album/add")
    public Object addShareAlbumComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long shareId,
            @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.addFeedReply(replys, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_ALBUM, uid, toId, 0);
        } catch (Exception e) {
            logger.error("Add share comment error", e);
        }
        return "@";
    }

    @Post("share/album/del")
    public Object delShareAlbumComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long shareId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.removeFeedReply(replys, replyId, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_ALBUM, uid, 0);
        } catch (Exception e) {
            logger.error("Remove share comment error", e);
        }
        return "@";
    }

    @Post("share/photo/add")
    public Object addSharePhotoComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long shareId,
            @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.addFeedReply(replys, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_PHOTO, uid, toId, 0);
        } catch (Exception e) {
            logger.error("Add share comment error", e);
        }
        return "@";
    }

    @Post("share/photo/del")
    public Object delSharePhotoComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long shareId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.removeFeedReply(replys, replyId, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_PHOTO, uid, 0);
        } catch (Exception e) {
            logger.error("Remove share comment error", e);
        }
        return "@";
    }

    @Post("share/link/add")
    public Object addShareLinkComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long shareId,
            @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.addFeedReply(replys, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_LINK, uid, toId, 0);
        } catch (Exception e) {
            logger.error("Add share comment error", e);
        }
        return "@";
    }

    @Post("share/link/del")
    public Object delShareLinkComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long shareId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.removeFeedReply(replys, replyId, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_LINK, uid, 0);
        } catch (Exception e) {
            logger.error("Remove share comment error", e);
        }
        return "@";
    }

    @Post("share/music/add")
    public Object addShareMusicComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long shareId,
            @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.addFeedReply(replys, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_MUSIC, uid, toId, 0);
        } catch (Exception e) {
            logger.error("Add share comment error", e);
        }
        return "@";
    }

    @Post("share/music/del")
    public Object delShareMusicComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long shareId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.removeFeedReply(replys, replyId, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_MUSIC, uid, 0);
        } catch (Exception e) {
            logger.error("Remove share comment error", e);
        }
        return "@";
    }

    @Post("share/video/add")
    public Object addShareVideoComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long shareId,
            @Param("to_id") int toId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.addFeedReply(replys, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_VIDEO, uid, toId, 0);
        } catch (Exception e) {
            logger.error("Add share comment error", e);
        }
        return "@";
    }

    @Post("share/video/del")
    public Object delShareVideoComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long shareId,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            feedReplyService.removeFeedReply(replys, replyId, shareId,
                    (int) FeedDefUtil.PAGE_SHARE_VIDEO, uid, 0);
        } catch (Exception e) {
            logger.error("Remove share comment error", e);
        }
        return "@";
    }

    @Post("share/generic/add")
    public Object addShareComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("sourceId") long shareId,
            @Param("ownerId") int shareOwnerId, @Param("to_id") int toId,
            @Param("feedtype") int feedType,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            // 新增LOG
            LogEntry log = LogEntryUtil.buildLogEntry(shareOwnerId, uid,
                    shareId, "TYPE_XOA_FEED", LogType.SHARE_COMMIT, null);
            RemoteLogService.getInstance().doLog(log);

            Long pageFeedType = FeedTypeMap.getMappedPageFeed(feedType);
            if (null == pageFeedType) {
                throw new RuntimeException(
                        "Can not find page feed type, in type(" + feedType
                                + ")");
            }
            feedReplyService.addFeedReply(replys, shareId,
                    pageFeedType.intValue(), uid, toId, shareOwnerId);
        } catch (Exception e) {
            logger.error("Add share comment error", e);
        }
        return "@";
    }

    @Post("share/generic/del")
    public Object delShareComment(@Param("user-id") int realUid,
            @Param("uid") int uid, @Param("replyId") long replyId,
            @Param("sourceId") long shareId,
            @Param("ownerId") int shareOwnerId,
            @Param("feedtype") int feedType,
            @JsonParam @Param("replys") ReplyContainer replys) {
        try {
            Long pageFeedType = FeedTypeMap.getMappedPageFeed(feedType);
            if (null == pageFeedType) {
                throw new RuntimeException(
                        "Can not find page feed type, in type(" + feedType
                                + ")");
            }
            feedReplyService.removeFeedReply(replys, replyId, shareId,
                    pageFeedType.intValue(), uid, shareOwnerId);
        } catch (Exception e) {
            logger.error("Remove share comment error", e);
        }
        return "@";
    }
}

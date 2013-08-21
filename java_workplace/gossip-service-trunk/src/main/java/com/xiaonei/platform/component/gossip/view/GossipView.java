package com.xiaonei.platform.component.gossip.view;

import java.io.Serializable;
import java.util.Date;

import org.apache.commons.lang.StringUtils;

import com.renren.app.at.model.AtFormatType;
import com.renren.doingubb.tool.DoingUbbReplace;
import com.xiaonei.platform.component.gossip.biz.GossipBiz;
import com.xiaonei.platform.component.gossip.util.GossipUtils;
import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.utility.AlbumUtil;
import com.xiaonei.ugc.ubb.UBBLogic;

/**
 * 
 * LeaveWordPresent.java
 * 
 * @author drizzt drizzt.wang@gmail.com 2006-6-24 0:17:33
 */
public class GossipView implements Serializable {

    public static final String xiaonei_whisper = "<xiaonei_only_to_me/>";

    public static final String xiaonei_wap = "<xiaonei_wap/>";

    public static final String gift_img = "<xiaonei_gift";

    public static final String wap_gift_img = "<xiaonei_wap_gift";

    //<color:ffffff:color/>
    public static final String color_begin = "<span style=\"color:#";

    public static final String color_end = "</span>";

    public String getFilterOriginalBody() {
        return GossipBiz.getFilterOriginalBody(body);
    }

    private static final long serialVersionUID = 1420663283064233377L;

    private long id;

    private int guestId;

    private int chatId;

    private String chatName;

    private String guestName;

    private String guestUniv;

    private String body;

    private boolean isVip;

    private Date time;

    private String tinyUrl;

    private boolean online;

    private boolean keepUse;

    private boolean charity;

    private boolean superEmotion;

    //@信息
//    private MentionInfo mentionInfo;

    public int getChatId() {
        return chatId;
    }

    public void setChatId(int chatId) {
        this.chatId = chatId;
    }

    public String getChatName() {
        return chatName;
    }

    public void setChatName(String chatName) {
        this.chatName = chatName;
    }

    public boolean isSuperEmotion() {
        return superEmotion;
    }

    public void setSuperEmotion(boolean superEmotion) {
        this.superEmotion = superEmotion;
    }

    public boolean isCharity() {
        return charity;
    }

    public void setCharity(boolean charity) {
        this.charity = charity;
    }

    public boolean isKeepUse() {
        return keepUse;
    }

    public void setKeepUse(boolean keepUse) {
        this.keepUse = keepUse;
    }

    private int owner;

    public int getOwner() {
        return owner;
    }

    public void setOwner(int owner) {
        this.owner = owner;
    }

    private String headUrl;

    private String largeUrl;

    public String getHeadUrl() {
        if (StringUtils.isNotBlank(headUrl)) {
            return AlbumUtil.getPhotoURL(headUrl);
        }
        return "";
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }

    public String getLargeUrl() {
        if (StringUtils.isNotBlank(largeUrl)) {
            return AlbumUtil.getPhotoURL(largeUrl);
        }
        return "";
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    public static long getSerialVersionUID() {
        return serialVersionUID;
    }

    public boolean isWap() {
        return body.contains(xiaonei_wap) || body.contains(wap_gift_img);
    }

    public boolean isWhisper() {
        return body.contains(xiaonei_whisper) || chatId > 0;
    }

    public String getColor() {
        String color = StringUtils.substringBetween(body, "<color:", ":color/>");
        return color == null ? "000000" : color;
    }

    public String getGiftImg() {
        String img = "about:blank";
        if (isGift()) {
            int begin = body.indexOf(gift_img);
            if (begin != -1) {
                begin += gift_img.length() + 6;
            } else {
                begin = body.indexOf(wap_gift_img);
                begin += wap_gift_img.length() + 6;
            }
            if (begin != -1) {
                int end = body.indexOf(".gif\"/>");
                if (end == -1) {
                    end = body.indexOf(".png\"/>");
                }
                try {
                    if (end != -1) {
                        img = body.substring(begin, end + 4);
                    } else {
                        System.out.println("error whisper body:" + body);
                        System.out.println("begin:" + begin + "  end:" + end);
                    }
                } catch (Exception e) {}
            }
        }
        return img;
    }

    public boolean isGift() {
        return body.contains(gift_img) || body.contains(wap_gift_img);
    }

    public boolean isVip() {
        return isVip;
    }

    public void setVip(boolean isVip) {
        this.isVip = isVip;
    }

    public boolean isOnline() {
        return online;
    }

    public void setOnline(boolean online) {
        this.online = online;
    }

    public String getBody() {
        return body;
    }

    /**
     * 经过ubb转换，包含html的数据
     * 
     * @return
     */
    public String getHtmlBody() {
    	String ubbBody = null;
//      if (isSuperEmotion()) {
//          ubbBody = UBBLogic.getInstance(OpiConstants.urlStatic).replaceForVip(body,
//                  UBBLogic.GOSSIP);
//      } else {
//          ubbBody = UBBLogic.getInstance(OpiConstants.urlStatic).replaceForNorm(body,
//                  UBBLogic.GOSSIP);
//      }
      ubbBody = DoingUbbReplace.getInstance().replaceUBB(body,
            UBBLogic.GOSSIP);

      try {
          String result = GossipUtils.formartAt(ubbBody, AtFormatType.HREF_NAME);
          return (StringUtils.isNotBlank(result)) ? result : ubbBody;
      } catch (Exception e) {
          return ubbBody;
      }
    }

    public String getFilterdBody() {
        if (chatId == 0) {
            String color = getColor();
            StringBuffer buffer = new StringBuffer();
            buffer.append(GossipView.color_begin + color + "\">");
            String tmpBody = getHtmlBody();
            if (tmpBody.contains(GossipView.xiaonei_whisper)) {
                tmpBody = tmpBody.substring(0, tmpBody.indexOf(GossipView.xiaonei_whisper));
            }
            buffer.append(tmpBody);
            buffer.append(GossipView.color_end);
            return buffer.toString();
        } else {
            return getHtmlBody();
        }
    }

    public void setBody(String body) {
        this.body = body;
    }

    public int getGuestId() {
        return guestId;
    }

    public void setGuestId(int guestId) {
        this.guestId = guestId;
    }

    public String getGuestName() {
        return guestName;
    }

    public void setGuestName(String guestName) {
        this.guestName = guestName;
    }

    public String getGuestUniv() {
        return guestUniv;
    }

    public void setGuestUniv(String guestUniv) {
        this.guestUniv = guestUniv;
    }

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public String getTinyUrl() {
        return HeadUrlUtil.getHeadFullUrl(tinyUrl);
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

//    public MentionInfo getMentionInfo() {
//        return mentionInfo;
//    }
//
//    public void setMentionInfo(MentionInfo mentionInfo) {
//        this.mentionInfo = mentionInfo;
//    }

}

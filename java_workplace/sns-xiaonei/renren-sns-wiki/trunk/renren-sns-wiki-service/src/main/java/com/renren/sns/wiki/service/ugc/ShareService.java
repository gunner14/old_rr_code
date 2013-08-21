package com.renren.sns.wiki.service.ugc;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.app.share.model.ShareComment;
import com.renren.doingubb.tool.DoingUbbReplace;
import com.renren.sns.wiki.delegator.xoa.AtXoaDelegator;
import com.xiaonei.platform.core.head.HeadUrlUtil;

@Service
public class ShareService {

    @Autowired
    private AtXoaDelegator atXoaDelegator;

    public Map<String, Object> convertShareCommentToMap(ShareComment shareComment) {

        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");

        Map<String, Object> map = new HashMap<String, Object>();
        map.put("id", shareComment.getId());
        String body = shareComment.getBody();
        //UBB表情替换
        body = DoingUbbReplace.getInstance().replaceUBB(body);
        //@替换
        body = atXoaDelegator.parseAt(body);

        map.put("body", body);
        //        map.put("author", shareComment.get);
        Date date = shareComment.getTime();
        String strDate = "";
        if (date != null) {
            strDate = sdf.format(date);
        }
        map.put("time", strDate);
        String wikiWords = shareComment.getWikiWords();
        map.put("wikiWords", wikiWords == null ? "" : wikiWords);
        //        map.put("whisper", shareComment.);
        map.put("isVip", shareComment.isVip());
        //        map.put("keepUse", shareComment.);
        map.put("name", shareComment.getCreatorName());
        map.put("owner", shareComment.getCreator());
        //        map.put("code", shareComment.);
        String headUrl = shareComment.getCreatorHead();
        String headFullUrl = HeadUrlUtil.getHeadFullUrl(headUrl);
        map.put("headUrl", headFullUrl);
        //        map.put("", shareComment.);
        return map;
    }

}

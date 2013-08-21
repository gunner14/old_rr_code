package com.xiaonei.page.xoa.controllers.pagefeed.builder;

import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.log4j.Logger;

import xce.feed.FeedReply;
import xce.feed.FeedTypeConfigNotFoundException;
import xce.feed.NotAllowedToSendException;
import MyUtil.MceException;

import com.xiaonei.page.xoa.domain.feed.StatusFeed;
import com.xiaonei.xce.feed.FeedCreatorAdapter;

/**
 * 此FeedBuilder_2008_1用于构建发送stype为2008，数据版本为1的新鲜事 新鲜事说明：Page更新状态
 * 
 * @author antonio
 * 
 */

public class StatusBuilder {

    /** 新鲜事的类型 */
    public final static int stype = 2008;

    /** 新鲜事的版本号 */
    public final static int version = 1;

    private Map<String, String> _data = new HashMap<String, String>();

    private Map<String, Boolean> _flag = new HashMap<String, Boolean>();

    private final static StatusBuilder instance = new StatusBuilder();

    public final static StatusBuilder getInstance() {
        return instance;
    }

    private StatusBuilder() {
        _flag.put("time", false);
        _flag.put("origin.type", false);
        _flag.put("origin.url", true);
        _flag.put("origin.title", true);
        _flag.put("origin.image", true);
        _flag.put("from.id", false);
        _flag.put("from.name", false);
        _flag.put("from.url", false);
        _flag.put("from.certify.icon", true);
        _flag.put("from.certify.title", true);
        _flag.put("from.tinyimg", false);
        _flag.put("status.id", false);
        _flag.put("status.title", false);
        _flag.put("status.web.title", true);
        _flag.put("status.raw", false);
        _flag.put("status.time", false);
        _flag.put("status.src", true);
        _flag.put("status.streamID", true);
        _flag.put("status.from.type", true);
        _flag.put("status.from.id", true);
        _flag.put("status.from.name", true);
        _flag.put("status.from.certify.icon", true);
        _flag.put("status.from.certify.title", true);
        _flag.put("status.re.id", false);
        _flag.put("status.re.title", false);
        _flag.put("status.re.raw", false);
        _flag.put("status.re.src", true);
        _flag.put("lbs.id", true);
        _flag.put("lbs.pid", true);
        _flag.put("lbs.content", true);
        _flag.put("lbs.longitude", true);
        _flag.put("lbs.latitude", true);
        _flag.put("stat.id", true);
        _flag.put("stat.level", true);
        _flag.put("minigroup.name", true);
        _flag.put("minigroup.url", true);
        _flag.put("minigroup.id", true);
    }

    boolean CheckUrl(String url) {
        if (url.isEmpty()) {
            return true;
        }
        Pattern p = Pattern.compile("^http://[^#]+$");
        Matcher m = p.matcher(url);
        if (m.find()) {
            return true;
        }
        System.err.println("URL格式不正确 url:" + url);
        return false;
    }

    /**
     * @param time comment:时间戳
     */
    public void SetKey_time(long time) {

        _data.put("time", String.valueOf(time));
        _flag.put("time", true);
    }

    /**
     * @param origin_type comment:类型
     */
    public void SetKey_origin_type(String origin_type) {

        _data.put("origin.type", String.valueOf(origin_type));
        _flag.put("origin.type", true);
    }

    /**
     * @param origin_url comment:链接地址
     */
    public void SetKey_origin_url(String origin_url) {

        if (!CheckUrl(origin_url)) {
            _flag.put("origin.url", false);
            return;
        }

        _data.put("origin.url", String.valueOf(origin_url));
        _flag.put("origin.url", true);
    }

    /**
     * @param origin_title comment:链接文本
     */
    public void SetKey_origin_title(String origin_title) {

        _data.put("origin.title", String.valueOf(origin_title));
        _flag.put("origin.title", true);
    }

    /**
     * @param origin_image comment:图片地址
     */
    public void SetKey_origin_image(String origin_image) {

        if (!CheckUrl(origin_image)) {
            _flag.put("origin.image", false);
            return;
        }

        _data.put("origin.image", String.valueOf(origin_image));
        _flag.put("origin.image", true);
    }

    /**
     * @param from_id comment:发送者ID
     */
    public void SetKey_from_id(long from_id) {

        _data.put("from.id", String.valueOf(from_id));
        _flag.put("from.id", true);
    }

    /**
     * @param from_name comment:发送者名字
     */
    public void SetKey_from_name(String from_name) {

        _data.put("from.name", String.valueOf(from_name));
        _flag.put("from.name", true);
    }

    /**
     * @param from_url comment:发送者主页地址
     */
    public void SetKey_from_url(String from_url) {

        _data.put("from.url", String.valueOf(from_url));
        _flag.put("from.url", true);
    }

    /**
     * @param from_certify_icon comment:认证icon
     */
    public void SetKey_from_certify_icon(String from_certify_icon) {

        if (!CheckUrl(from_certify_icon)) {
            _flag.put("from.certify.icon", false);
            return;
        }

        _data.put("from.certify.icon", String.valueOf(from_certify_icon));
        _flag.put("from.certify.icon", true);
    }

    /**
     * @param from_certify_title comment:认证title
     */
    public void SetKey_from_certify_title(String from_certify_title) {

        _data.put("from.certify.title", String.valueOf(from_certify_title));
        _flag.put("from.certify.title", true);
    }

    /**
     * @param from_tinyimg comment:发送者小头像
     */
    public void SetKey_from_tinyimg(String from_tinyimg) {

        if (!CheckUrl(from_tinyimg)) {
            _flag.put("from.tinyimg", false);
            return;
        }

        _data.put("from.tinyimg", String.valueOf(from_tinyimg));
        _flag.put("from.tinyimg", true);
    }

    /**
     * @param status_id comment:状态id
     */
    public void SetKey_status_id(long status_id) {

        _data.put("status.id", String.valueOf(status_id));
        _flag.put("status.id", true);
    }

    /**
     * @param status_title comment:状态title
     */
    public void SetKey_status_title(String status_title) {

        _data.put("status.title", String.valueOf(status_title));
        _flag.put("status.title", true);
    }

    /**
     * @param status_web_title comment:?
     */
    public void SetKey_status_web_title(String status_web_title) {

        _data.put("status.web.title", String.valueOf(status_web_title));
        _flag.put("status.web.title", true);
    }

    /**
     * @param status_raw comment:原数据
     */
    public void SetKey_status_raw(String status_raw) {

        _data.put("status.raw", String.valueOf(status_raw));
        _flag.put("status.raw", true);
    }

    /**
     * @param status_time comment:状态发送时间?
     */
    public void SetKey_status_time(String status_time) {

        _data.put("status.time", String.valueOf(status_time));
        _flag.put("status.time", true);
    }

    /**
     * @param status_src comment:?
     */
    public void SetKey_status_src(String status_src) {

        _data.put("status.src", String.valueOf(status_src));
        _flag.put("status.src", true);
    }

    /**
     * @param status_streamID comment:streamID
     */
    public void SetKey_status_streamID(String status_streamID) {

        _data.put("status.streamID", String.valueOf(status_streamID));
        _flag.put("status.streamID", true);
    }

    /**
     * @param status_from_type comment:状态拥有者类型
     */
    public void SetKey_status_from_type(String status_from_type) {

        _data.put("status.from.type", String.valueOf(status_from_type));
        _flag.put("status.from.type", true);
    }

    /**
     * @param status_from_id comment:状态拥有者Id
     */
    public void SetKey_status_from_id(long status_from_id) {

        _data.put("status.from.id", String.valueOf(status_from_id));
        _flag.put("status.from.id", true);
    }

    /**
     * @param status_from_name comment:状态拥有者名字
     */
    public void SetKey_status_from_name(String status_from_name) {

        _data.put("status.from.name", String.valueOf(status_from_name));
        _flag.put("status.from.name", true);
    }

    /**
     * @param status_from_certify_icon comment:认证icon
     */
    public void SetKey_status_from_certify_icon(String status_from_certify_icon) {

        if (!CheckUrl(status_from_certify_icon)) {
            _flag.put("status.from.certify.icon", false);
            return;
        }

        _data.put("status.from.certify.icon", String.valueOf(status_from_certify_icon));
        _flag.put("status.from.certify.icon", true);
    }

    /**
     * @param status_from_certify_title comment:认证title
     */
    public void SetKey_status_from_certify_title(String status_from_certify_title) {

        _data.put("status.from.certify.title", String.valueOf(status_from_certify_title));
        _flag.put("status.from.certify.title", true);
    }

    /**
     * @param status_re_id comment:?
     */
    public void SetKey_status_re_id(long status_re_id) {

        _data.put("status.re.id", String.valueOf(status_re_id));
        _flag.put("status.re.id", true);
    }

    /**
     * @param status_re_title comment:?
     */
    public void SetKey_status_re_title(String status_re_title) {

        _data.put("status.re.title", String.valueOf(status_re_title));
        _flag.put("status.re.title", true);
    }

    /**
     * @param status_re_raw comment:?
     */
    public void SetKey_status_re_raw(String status_re_raw) {

        _data.put("status.re.raw", String.valueOf(status_re_raw));
        _flag.put("status.re.raw", true);
    }

    /**
     * @param status_re_src comment:?
     */
    public void SetKey_status_re_src(String status_re_src) {

        _data.put("status.re.src", String.valueOf(status_re_src));
        _flag.put("status.re.src", true);
    }

    /**
     * @param lbs_id comment:lbsID
     */
    public void SetKey_lbs_id(long lbs_id) {

        _data.put("lbs.id", String.valueOf(lbs_id));
        _flag.put("lbs.id", true);
    }

    /**
     * @param lbs_pid comment:?
     */
    public void SetKey_lbs_pid(long lbs_pid) {

        _data.put("lbs.pid", String.valueOf(lbs_pid));
        _flag.put("lbs.pid", true);
    }

    /**
     * @param lbs_content comment:位置信息
     */
    public void SetKey_lbs_content(String lbs_content) {

        _data.put("lbs.content", String.valueOf(lbs_content));
        _flag.put("lbs.content", true);
    }

    /**
     * @param lbs_longitude comment:经度
     */
    public void SetKey_lbs_longitude(String lbs_longitude) {

        _data.put("lbs.longitude", String.valueOf(lbs_longitude));
        _flag.put("lbs.longitude", true);
    }

    /**
     * @param lbs_latitude comment:纬度
     */
    public void SetKey_lbs_latitude(String lbs_latitude) {

        _data.put("lbs.latitude", String.valueOf(lbs_latitude));
        _flag.put("lbs.latitude", true);
    }

    /**
     * @param stat_id comment:传播id
     */
    public void SetKey_stat_id(String stat_id) {

        _data.put("stat.id", String.valueOf(stat_id));
        _flag.put("stat.id", true);
    }

    /**
     * @param stat_level comment:传播层级
     */
    public void SetKey_stat_level(String stat_level) {

        _data.put("stat.level", String.valueOf(stat_level));
        _flag.put("stat.level", true);
    }

    /**
     * @param minigroup_name comment:小群名字
     */
    public void SetKey_minigroup_name(String minigroup_name) {

        _data.put("minigroup.name", String.valueOf(minigroup_name));
        _flag.put("minigroup.name", true);
    }

    /**
     * @param minigroup_url comment:url
     */
    public void SetKey_minigroup_url(String minigroup_url) {

        if (!CheckUrl(minigroup_url)) {
            _flag.put("minigroup.url", false);
            return;
        }

        _data.put("minigroup.url", String.valueOf(minigroup_url));
        _flag.put("minigroup.url", true);
    }

    /**
     * @param minigroup_id comment:小群ID
     */
    public void SetKey_minigroup_id(long minigroup_id) {

        _data.put("minigroup.id", String.valueOf(minigroup_id));
        _flag.put("minigroup.id", true);
    }
    
    /**
     * 
     * 调用此方法触发产生新鲜事，用于产生带回复的新鲜事，所以需要传FeedReply参数。
     * 
     * @param feed feed参数
     * @param reply reply参数
     * @throws IllegalArgumentException
     * @throws IllegalAccessException
     * @throws MceException
     */
    public void DispatchFeed(StatusFeed feed, FeedReply reply) throws IllegalArgumentException,
            IllegalAccessException, MceException {

        Class<? extends StatusFeed> clazz = feed.getClass();
        Field[] fields = clazz.getDeclaredFields();
        for (Field field : fields) {
            //设置访问标志
            field.setAccessible(true);
            String fieldName = field.getName();
            //替换
            fieldName = fieldName.replace('_', '.');
            String fieldValue = (String) field.get(feed);
            _data.put(fieldName, fieldValue);
            _flag.put(fieldName, true);
        }
        //TODO 这里需要一段逻辑来，验证传入的数据是否有缺失
        if(reply==null){
            FeedCreatorAdapter.getInstance().Create(stype, version, _data);
        }else{
            FeedCreatorAdapter.getInstance().CreateWithReply(stype, version, _data,reply);
        }
        
    }

    /**
     * 调用此方法触发产生新鲜事，用于产生带回复的新鲜事，所以需要传FeedReply参数
     */
    public void DispatchFeed(FeedReply reply) {
        StringBuffer buf = new StringBuffer();

        Iterator it = _flag.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<String, Boolean> entry = (Map.Entry<String, Boolean>) it.next();
            if (!entry.getValue()) {
                if (buf.length() == 0) {
                    buf.append("Any SetKey function not set, include keys:").append(entry.getKey());
                } else {
                    buf.append(",").append(entry.getKey());
                }

            }
        }
        if (buf.length() > 0) {
            System.err.println(buf.toString());
            return;
        }

        try {
            if (reply == null) {
                FeedCreatorAdapter.getInstance().Create(stype, version, _data);
            } else {
                FeedCreatorAdapter.getInstance().CreateWithReply(stype, version, _data, reply);
            }

        } catch (FeedTypeConfigNotFoundException e) {
            e.printStackTrace();
        } catch (NotAllowedToSendException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (MceException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    /**
     * 调用此方法触发产生新鲜事
     */
    public void DispatchFeed() {
        DispatchFeed(null);
    }
}

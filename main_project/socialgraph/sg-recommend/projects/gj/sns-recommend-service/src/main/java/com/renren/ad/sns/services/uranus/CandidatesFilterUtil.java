package com.renren.ad.sns.services.uranus;

import java.util.HashMap;
import java.util.Map;

public class CandidatesFilterUtil {

    /* 商品链接的前缀 */
    public static final Map<String, String> LINK_PREFIX = new HashMap<String, String>();
    static {
        LINK_PREFIX.put("http://item.taobao.com/item.htm?id=", "tb");
        LINK_PREFIX.put("http://www.woyo.com/shop/detail-", "wy");
        LINK_PREFIX.put("http://item.mbaobao.com/pshow-", "mbb");
        LINK_PREFIX.put("http://www.xzhuang.com/?product-", "xz");
        LINK_PREFIX.put("http://www.wowsai.com/goods/", "ws");
        LINK_PREFIX.put("http://www.kela.cn/", "kl");
        LINK_PREFIX.put("http://www.quwan.com/goods.php?id=", "qw");
        LINK_PREFIX.put("http://www.tooogooo.com/team.php?id=", "tg");
        LINK_PREFIX.put("http://www.5lux.com/goods.php?id=", "51");
        LINK_PREFIX.put("http://www.fionny.com/goods.php?id=", "fi");
        LINK_PREFIX.put("http://www.dbox.cn/deal.php?id=", "db");
        LINK_PREFIX.put("http://taourl.com/", "tu");
    }

    public static final String BAN_URL = "http://s.click.taobao.com";

    public static String getShortLink(String link) {
        String prefix = "";
        for (String linkPrefix : LINK_PREFIX.keySet()) {
            if (link.startsWith(linkPrefix)) {
                prefix = linkPrefix;
                break;
            }
        }

        return LINK_PREFIX.get(prefix) + link.subSequence(prefix.length(), link.length());
    }
}

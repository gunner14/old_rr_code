package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;

import org.json.JSONArray;
import org.json.JSONObject;

import com.meidusa.amoeba.util.StringUtil;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午04:36:40
 */
@Path("ajaxGetNoteAction")
public class AjaxGetNoteActionController {

    @Get
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        String tips = (String) inv.getRequest().getParameter("tips");
        JSONArray jsonArray = new JSONArray();
        MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(
                MemCacheKeys.pool_guide);
        if (tips != null) {
            String tipsArray[] = StringUtil.split(tips, ",");
            for (int i = 0; i < tipsArray.length; i++) {
                if (mem.get(tipsArray[i]) == null) {
                    JSONObject json = new JSONObject();
                    json.put("tipid", tipsArray[i]);
                    jsonArray.put(json);
                }
            }
        }
        inv.addModel("tipsJsonArray", jsonArray);
        return "/views/guide/xf/common/ajax/guide_get_note.jsp";
    }
}

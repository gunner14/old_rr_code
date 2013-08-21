package com.xiaonei.commons.interceptors.access;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.wiki.WikiUserIdsManager;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import org.springframework.beans.factory.annotation.Autowired;

import java.lang.annotation.Annotation;

/**
 * Wiki用户开放策略拦截器
 * User: shunlong.jin/David
 * Date: 12-5-29
 * Time: 下午3:21
 */
public class WikiUserInterceptor extends ControllerInterceptorAdapter {
    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiUserIdsManager wikiUserIdsManager;

    private static final String wiki = "is_wiki_user";

    public WikiUserInterceptor() {
        this.setPriority(-2000);
    }

    @Override
    public Object before(final Invocation inv) throws Exception {
        try {
            if (this.hostHolder == null) {
                return true;
            }
            final User user = this.hostHolder.getUser();
            if (user == null) {
                return true;
            }

            if (this.wikiUserIdsManager == null) {
                return true;
            }
            //edit by shunlong.jin 因为白名单用户太多了，用到了userState，这里修改了接口
            if (this.wikiUserIdsManager.isWikiUser(user)) {
                inv.getRequest().setAttribute(wiki, true);
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return true;
    }


    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return IgnorePassportValidation.class;
    }

}

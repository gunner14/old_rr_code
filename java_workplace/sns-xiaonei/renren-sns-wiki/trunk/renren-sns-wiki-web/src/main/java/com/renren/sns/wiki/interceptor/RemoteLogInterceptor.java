package com.renren.sns.wiki.interceptor;

import java.lang.annotation.Annotation;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.xml.DOMConfigurator;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.util.Assert;

import com.renren.sns.wiki.utils.remotelog.LogWhenType;
import com.renren.sns.wiki.utils.remotelog.RemoteLogUtil;
import com.renren.sns.wiki.utils.remotelog.annotation.RemoteLog;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

/**
 * 远程日志拦截器，用来记录远程日志
 * 
 * @author yi.li@renren-inc.com since 2012-3-20
 */
@Interceptor(oncePerRequest = true)
public class RemoteLogInterceptor extends ControllerInterceptorAdapter {

    private Log logger = LogFactory.getLog(this.getClass());

    private Log remoteLogger = LogFactory.getLog("wikiremotelog");

    private final static String REMOTELOG_SEPERATOR = "||";

    public static final SimpleDateFormat DF = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private HostHolder hostHolder;

    @Override
    public int getPriority() {
        return 1;
    }

    @Override
    protected Class<? extends Annotation> getRequiredAnnotationClass() {
        return RemoteLog.class;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        RemoteLog logAnnotation = inv.getMethod().getAnnotation(RemoteLog.class);
        Assert.notNull(logAnnotation);

        if (logAnnotation.logWhen() != LogWhenType.ACCESS) {
            return true;
        }

        doLog(inv, logAnnotation);

        return true;
    }

    @Override
    public Object after(Invocation inv, Object instruction) throws Exception {
        RemoteLog logAnnotation = inv.getMethod().getAnnotation(RemoteLog.class);
        Assert.notNull(logAnnotation);

        if (logAnnotation.logWhen() == LogWhenType.COMPELETE && successFinished(inv)) {
            doLog(inv, logAnnotation);
        }

        return super.after(inv, instruction);
    }

    private void doLog(Invocation inv, RemoteLog annotation) {
        try {
            int wikiId = wikiHolder.getWiki() == null ? 0 : wikiHolder.getWiki().getId();
            int userId = hostHolder.getUser() == null ? 0 : hostHolder.getUser().getId();

            long sourceId = 0L;
            if (!StringUtils.isEmpty(annotation.sourceParam())) {
                String sourceStr = (String) getParameter(inv, annotation.sourceParam());
                if (!StringUtils.isEmpty(sourceStr)) {
                    try {
                        sourceId = Long.parseLong(sourceStr);
                    } catch (Exception e) {}
                }
            }

            // TODO make it real or not used
            boolean isFans = false;
            String attach = "";
            String ip = BaseActionUtil.getClientIP(inv.getRequest());
            String ref = inv.getRequest().getHeader("Referer");
            String curUri = inv.getRequestPath().getUri();
            String ua = inv.getRequest().getHeader("User-Agent");

            StringBuilder sb = new StringBuilder();
            sb.append(wikiId).append(REMOTELOG_SEPERATOR);
            sb.append(userId).append(REMOTELOG_SEPERATOR);
            sb.append(DF.format(new Date())).append(REMOTELOG_SEPERATOR);
            sb.append(ip).append(REMOTELOG_SEPERATOR);
            sb.append(annotation.type().getTypeName()).append(REMOTELOG_SEPERATOR);
            sb.append(sourceId).append(REMOTELOG_SEPERATOR);
            sb.append(isFans).append(REMOTELOG_SEPERATOR);
            sb.append(attach).append(REMOTELOG_SEPERATOR);
            sb.append(ref).append(REMOTELOG_SEPERATOR);
            sb.append(ua).append(REMOTELOG_SEPERATOR);
            // 除了ref当前访问的url也应该记录，以便于排查问题
            sb.append(curUri).append(REMOTELOG_SEPERATOR);

            // 额外参数
            String[] extras = annotation.extras();
            if (null != extras && extras.length > 0) {
                for (String param : extras) {
                    Object value = getParameter(inv, param);
                    sb.append(value == null ? "" : value.toString()).append(REMOTELOG_SEPERATOR);
                }
            }

            if (logger.isDebugEnabled()) {
                logger.debug("Remote Log Info : " + sb.toString());
            }
            remoteLogger.info(sb.toString());
        } catch (Exception e) {
            logger.error("Record remote log error.", e);
        }
    }

    private Object getParameter(Invocation inv, String parameterName) {
        String result = inv.getParameter(parameterName);
        if (result != null) {
            return result;
        }

        return inv.getModel(parameterName);
    }

    /**
     * 判断业务是否成功执行
     */
    private boolean successFinished(Invocation inv) {
        return RemoteLogUtil.isDone(inv.getModel());
    }

    public static void main(String args[]) {
        DOMConfigurator
                .configure("D:\\projects\\renren-sns-wiki\\renren-sns-wiki-web\\src\\main\\webapp\\WEB-INF\\log4j.xml");
        Log log = LogFactory.getLog("wikiremotelog");
        for (int i = 0; i < 100000; i++) {
            log.info("xxdfd||www" + i);
            if (i % 10000 == 0) {
                System.out.print("*");
            }
        }

        try {
            Thread.sleep(300000);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}

/**
 * 
 */
package com.xiaonei.commons.interceptors.access.track.decision;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.paramresolver.ParamMetaData;
import net.paoding.rose.web.paramresolver.ParamResolver;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public class DecisionResolver implements ParamResolver {

    private static final Log logger = LogFactory.getLog(DecisionResolver.class);

    public static final String DECISION_KEY = DecisionResolver.class.getName();

    @Override
    public Object resolve(Invocation inv, ParamMetaData metaData) throws Exception {
        return getDecision(inv);
    }

    /**
     * 从当前Invocation里的父级Invocation获取Decision
     * 
     * @param headInv
     */
    public static Decision getDecision(Invocation inv) {
        Invocation headInv = DecisionUtil.getHeadInvocation(inv);
        if (headInv.getRequest().getAttribute(DECISION_KEY) == null) {
            synchronized (headInv) {
                if (headInv.getRequest().getAttribute(DECISION_KEY) == null) {
                    Decision decision = new DecisionImpl();
                    logger.debug("Create new Decision:" + decision);
                    headInv.getRequest().setAttribute(DECISION_KEY, decision);
                }
            }
        }
        return (Decision) headInv.getRequest().getAttribute(DECISION_KEY);
    }

    @Override
    public boolean supports(ParamMetaData metaData) {
        return Decision.class.equals(metaData.getParamType());
    }

}

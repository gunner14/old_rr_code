/**
 * 
 */
package com.xiaonei.commons.interceptors.access.track.decision;

import net.paoding.rose.web.Invocation;

/**
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public class DecisionUtil {

   public static Invocation getHeadInvocation(Invocation inv) {
        Invocation head = inv;
        while (head.getPreInvocation() != null) {
            head = head.getPreInvocation();
        }
        return head;
    }    
}

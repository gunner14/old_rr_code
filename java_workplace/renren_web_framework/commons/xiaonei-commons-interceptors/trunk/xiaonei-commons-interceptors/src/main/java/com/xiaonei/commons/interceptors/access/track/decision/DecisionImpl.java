/**
 * 
 */
package com.xiaonei.commons.interceptors.access.track.decision;

/**
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public class DecisionImpl implements Decision {

    StringBuffer sb = new StringBuffer();

    @Override
    public void makeDecision(String decision) {
        sb.append(decision);
        sb.append("$$");
    }

    @Override
    public Object getDecision() {
        return sb.toString();
    }
    
    @Override
    public String toString() {
        return super.toString()+sb.toString();
    }

}

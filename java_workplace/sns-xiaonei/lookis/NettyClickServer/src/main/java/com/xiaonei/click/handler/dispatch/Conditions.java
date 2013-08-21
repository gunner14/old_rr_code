/**
 * 
 */
package com.xiaonei.click.handler.dispatch;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * 树枝
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class Conditions implements Condition {

    List<Condition> childs = new ArrayList<Condition>();

    @Override
    public boolean check(String URL) {
        Iterator<Condition> iter = childs.iterator();
        while (iter.hasNext()) {
            if (!iter.next().check(URL)) {
                return false;
            }
        }
        return true;
    }

    public void add(Condition condition) {
        childs.add(condition);
    }

    public void remove(Condition condition) {
        childs.remove(condition);
    }

    public int size() {
        return childs.size();
    }

    @Override
    public Condition getCondition() {
        return this;
    }

}

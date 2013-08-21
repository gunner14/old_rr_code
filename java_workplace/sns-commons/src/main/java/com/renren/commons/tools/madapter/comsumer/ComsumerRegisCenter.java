package com.renren.commons.tools.madapter.comsumer;

import java.util.ArrayList;
import java.util.List;

public class ComsumerRegisCenter {

    private final static int maxListener = 5;

    private final static List<CallModelListener> listeners = new ArrayList<CallModelListener>();

    public synchronized static boolean addListener(final CallModelListener listener) {
        if (listeners.size() >= maxListener) {
            return false;
        }
        return listeners.add(listener);
    }

    public synchronized static boolean removeListener(final CallModelListener listener) {
        return listeners.remove(listener);
    }

    public synchronized static List<CallModelListener> getListeners() {
        return listeners;
    }
}

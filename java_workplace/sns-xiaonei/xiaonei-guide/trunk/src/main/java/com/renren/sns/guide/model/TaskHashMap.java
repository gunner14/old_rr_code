package com.renren.sns.guide.model;

import java.util.HashMap;
import java.util.Map;

public class TaskHashMap {

    private TaskHashMap() {
    }

    private static TaskHashMap instance = null;

    public static synchronized TaskHashMap getInstance() {
        if (instance == null) {
            instance = new TaskHashMap();
        }
        return instance;
    }

    private static Map<Integer, GuideNewTaskView> map = new HashMap<Integer, GuideNewTaskView>();

    private static void reloadMap() {
        map = new HashMap<Integer, GuideNewTaskView>();
        GuideNewTaskView m1 = new GuideNewTaskView(1, "加认识的人为好友", 1, 5, "狗骨头200根");
        GuideNewTaskView m2 = new GuideNewTaskView(2, "加明星为好友", 0, 3, "");
        GuideNewTaskView m3 = new GuideNewTaskView(3, "查看好友的好友", 1, 4, "农场装扮套装");
        GuideNewTaskView m4 = new GuideNewTaskView(4, "查看新鲜事", 0, 3, "");
        GuideNewTaskView m5 = new GuideNewTaskView(5, "查看游戏大厅", 1, 3, "VIP7日体验");
        map.put(1, m1);
        map.put(2, m2);
        map.put(3, m3);
        map.put(4, m4);
        map.put(5, m5);
    }

    public Map<Integer, GuideNewTaskView> getTaskHashMap() {
        if (map == null || map.size() == 0) {
            reloadMap();
        }
        return map;
    }
}

package com.xiaonei.click;

public class Test {

    private static volatile Test _instance = null;

    private Integer a = null;

    private Test() {
        a = 1;
    }

    public static Test getInstance() {
        if (_instance == null) {
            synchronized (Test.class) {
                if (_instance == null) {
                    _instance = new Test();
                }
            }
        }
        return _instance;
    }

}

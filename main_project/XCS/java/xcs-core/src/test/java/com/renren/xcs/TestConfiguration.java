package com.renren.xcs;

import java.util.List;

public class TestConfiguration implements Configuration {

    @Override
    public String getClientUsername() {
        return "test";
    }

    @Override
    public String getRoot() {
        return "online";
    }

    @Override
    public String getClientPassword() {
        return "test";
    }

    @Override
    public String getServerUsername() {
        return "test";
    }

    @Override
    public String getServerPassword() {
        return "test";
    }

    @Override
    public String getCluster() {
        return "127.0.0.1:2181";
    }

    @Override
    public List<String> getServerAlarmPhones() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public List<String> getServerAlarmEmails() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public List<String> getClientAlarmPhones() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public List<String> getClientAlarmEmails() {
        // TODO Auto-generated method stub
        return null;
    }

}

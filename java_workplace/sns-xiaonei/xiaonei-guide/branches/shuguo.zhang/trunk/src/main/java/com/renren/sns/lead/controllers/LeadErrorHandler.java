package com.renren.sns.lead.controllers;

import net.paoding.rose.web.ControllerErrorHandler;
import net.paoding.rose.web.Invocation;


public class LeadErrorHandler implements ControllerErrorHandler {

    @Override
    public Object onError(Invocation inv, Throwable ex) throws Throwable {
        System.err.println("lead window error -------->  inshion inshion inshion!"+" ex:"+ex);
        //ex.printStackTrace();
        return "";//return empty (not null) for keeping portal running
    }

    @Override
    public String toString() {
        return "LeadErrorHandler: just print error stack";
    }

}

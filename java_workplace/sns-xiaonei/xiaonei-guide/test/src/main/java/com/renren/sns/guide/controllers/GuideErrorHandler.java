package com.renren.sns.guide.controllers;

import net.paoding.rose.web.ControllerErrorHandler;
import net.paoding.rose.web.Invocation;


public class GuideErrorHandler implements ControllerErrorHandler {

    @Override
    public Object onError(Invocation inv, Throwable ex) throws Throwable {
        System.err.println("guide window error -------->");
        ex.printStackTrace();
        return "";//return empty (not null) for keeping portal running
    }

    @Override
    public String toString() {
        return "GuideErrorHandler: just print error stack";
    }

}

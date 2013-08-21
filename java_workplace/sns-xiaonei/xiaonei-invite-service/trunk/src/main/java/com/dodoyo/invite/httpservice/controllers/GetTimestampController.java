package com.dodoyo.invite.httpservice.controllers;

import java.util.Date;

import net.paoding.rose.web.annotation.rest.Post;

import com.dodoyo.invite.httpservice.logic.ResponseUtils;

public class GetTimestampController{

    @Post
    public String post() {
        String timestampString = "<timestamp>" + new Date().getTime() + "</timestamp>";
        timestampString = ResponseUtils.getInstance().getResponseXML(new StringBuffer(timestampString), 0);
        return "@" + timestampString;
    }

}
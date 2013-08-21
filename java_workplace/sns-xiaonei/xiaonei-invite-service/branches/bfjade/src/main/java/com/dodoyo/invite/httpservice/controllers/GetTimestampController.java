package com.dodoyo.invite.httpservice.controllers;

import com.dodoyo.invite.httpservice.logic.ResponseUtils;
import java.util.Date;

public class GetTimestampController
{

    public String post() 
    {

        String timestampString = "<timestamp>" + new Date().getTime() + "</timestamp>";
        timestampString = ResponseUtils.getInstance().getResponseXML(new StringBuffer(timestampString), 0);
        return "@" + timestampString;
    }
    
    public String get()
    {
    	return post();
    }

}
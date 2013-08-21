package com.dodoyo.invite.httpservice.controllers;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Post;

public class RecvPostDatasController {

	//private Invocation iv;
	
    @Post
    public String post(Invocation iv)
    {
    	StringBuffer output = new StringBuffer("");
    	try
    	{
	    	HttpServletRequest request = iv.getRequest();
	        InputStream is = request.getInputStream();
	        BufferedInputStream bis = new BufferedInputStream(is);
	        BufferedReader br = new BufferedReader(new InputStreamReader(bis));
	
	        String line = null;
	        line = br.readLine();
	        
	        output.append("<post_datas>");
	
	        while (line != null) {
	            output.append(line);
	            line = br.readLine();
	        }
	
	        br.close();
	
	        output.append("</post_datas>");
    	}
    	catch(Exception e)
    	{
    		e.printStackTrace(System.err);
    		output = new StringBuffer("");
    	}
        return "@" + output;
    }

}
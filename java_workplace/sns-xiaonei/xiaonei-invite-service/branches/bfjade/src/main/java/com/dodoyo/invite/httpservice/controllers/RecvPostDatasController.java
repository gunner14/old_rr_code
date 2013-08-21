package com.dodoyo.invite.httpservice.controllers;

import javax.servlet.http.HttpServletRequest;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class RecvPostDatasController {

	//private Invocation iv;
	
    public String post()
    {
    	String output = "";
    	try
    	{
	    	HttpServletRequest request = null;//iv.getRequest();
	        InputStream is = request.getInputStream();
	        BufferedInputStream bis = new BufferedInputStream(is);
	        BufferedReader br = new BufferedReader(new InputStreamReader(bis));
	
	        String line = null;
	        line = br.readLine();
	        
	        output += "<post_datas>";
	
	        while (line != null) {
	            output += line;
	            line = br.readLine();
	        }
	
	        br.close();
	
	        output += "</post_datas>";
    	}
    	catch(Exception e)
    	{
    		e.printStackTrace(System.err);
    		output = "";
    	}
        return "@" + output;
    }

}
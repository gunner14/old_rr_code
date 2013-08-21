package com.dodoyo.invite.httpservice.action;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class RecvPostDatasAction extends ServiceAbstractAction {

    @Override
    protected ActionForward hanleRequest(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws Exception {
        InputStream is = request.getInputStream();
        BufferedInputStream bis = new BufferedInputStream(is);
        BufferedReader br = new BufferedReader(new InputStreamReader(bis));

        String line = null;

        line = br.readLine();

        response.getWriter().print("<post_datas>");

        while (line != null) {
            response.getWriter().print(line);
            line = br.readLine();
        }

        br.close();

        response.getWriter().print("</post_datas>");
        return null;
    }

}
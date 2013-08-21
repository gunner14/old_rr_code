package com.xiaonei.click.controllers;

import java.net.MalformedURLException;
import java.sql.SQLException;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Get;

import org.json.JSONArray;

import com.xiaonei.click.dao.ClickDb;
import com.xiaonei.click.module.Click;
import com.xiaonei.click.module.ClickReq;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class ClickOnMeController {

    @Get
    public String index(@Param("c") ClickReq click) {

        if (click.getUrl() != null && click.getUrl().trim().length() > 0) {
            try {
                List<Click> cl = ClickDb.getInstance().getClick(click.getUrl());
                JSONArray ja = new JSONArray();
                for (Click click2 : cl) {
                    JSONArray joinner = new JSONArray();
                    joinner.put(click2.getX());
                    joinner.put(click2.getY());
                    ja.put(joinner);
                }
                return "@" + ja.toString();
            } catch (SQLException e) {
                e.printStackTrace();
                return "@[]";
            } catch (MalformedURLException e) {
                e.printStackTrace();
                return "@[]";
            }

        } else {
            return "@[]";
        }
    }
}

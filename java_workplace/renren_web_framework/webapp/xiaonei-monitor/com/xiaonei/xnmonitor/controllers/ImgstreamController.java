package com.xiaonei.xnmonitor.controllers;

import java.awt.Color;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;

import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.Rc;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Return;
import org.jrobin.core.RrdException;
import org.jrobin.graph.RrdGraph;
import org.jrobin.graph.RrdGraphDef;

import com.xiaonei.xnmonitor.utils.Global;
import com.xiaonei.xnmonitor.utils.ImgUtil;

public class ImgstreamController {

    // @HttpFeatures (contentType="img/png")
    @Return("@")
    public void getImg(Invocation inv, @Param("picType") final String picType,
            @Param("dayType") String dayType, @Param("logType") String logType,
            @Param("rrdFile") String rrdFile, @Param("childDir") String childDir,
            @Param("bigPic") String bigPic, @Param("dsName") String dsName) throws IOException,
            RrdException {
        String filePath = Global.JROBIN_RESULT_DIR;
        if (null != logType && !"".equals(logType)) {
            filePath = filePath + logType + "/";
        }
        if (null != childDir && childDir.length() > 1) {
            if (childDir.startsWith("/")) {
                childDir = childDir.substring(1, childDir.length());
            }
            if (childDir.endsWith("/")) {
                childDir = childDir.substring(0, childDir.length() - 1);
            }
            if (childDir.endsWith(".rrd")) {
                childDir = childDir.substring(0, childDir.length() - 4);
            }
            filePath += childDir + "/";
        }
        File file = new File(filePath + rrdFile);
        //		RrdGraphDef gDef = new RrdGraphDef();
        if (null == dayType || "".equals(dayType)) {
            dayType = "day";
        }
        RrdGraph rrdGraph = getRrddGraph(bigPic, dayType, rrdFile, dsName, file, picType);
        HttpServletResponse response = inv.getResponse();// Rc.response();

        response.setContentType("image/png");
        OutputStream output = response.getOutputStream();
        if ("true".equals(bigPic)) {
            output.write(rrdGraph.getPNGBytes(1000, 520));
            bigPic = "false";
        } else {
            output.write(rrdGraph.getPNGBytes(250, 110));
            bigPic = "true";
        }

        output.flush();
        output.close();
    }

    public RrdGraph getRrddGraph(String bigPic, String dayType, String rrdFile, String dsName,
            File file, String picType) throws RrdException {
        RrdGraphDef gDef = new RrdGraphDef();
        //        RrdDb db = new RrdDb(path, readOnly);
        ImgUtil.setTimePeriod(gDef, dayType);
        gDef.setVerticalLabel("Xiaonei Web Framework Team");
        gDef.setShowSignature(false);
        gDef.setTitle(rrdFile.substring(0, rrdFile.length() - 4));
        if ("max".equals(picType) || null == picType || "all".equals(picType)) {
            gDef.datasource("max", file.getAbsolutePath(), dsName, "MAX");
            if ("true".equals(bigPic)) {
                gDef.line("max", Color.GRAY, "MAX");
            } else {
                gDef.area("max", Color.GRAY, "MAX");
            }
            gDef.gprint("max", "MAX", "max:@2");
            gDef.gprint("max", "AVERAGE", "@2");
            gDef.gprint("max", "MIN", "@2@r");
            

        }

        if ("average".equals(picType) || null == picType || "all".equals(picType)) {
            gDef.datasource("average", file.getAbsolutePath(), dsName, "AVERAGE");
            if ("true".equals(bigPic)) {
                gDef.line("average", Color.GREEN, "AVERAGE", 2);
            } else {
                gDef.line("average", Color.GREEN, "AVERAGE");
            }
            gDef.gprint("average", "MAX", "average:@2");
            gDef.gprint("average", "AVERAGE", "@2");
            gDef.gprint("average", "MIN", "@2@r");
        }
        if ("min".equals(picType) || null == picType || "all".equals(picType)) {
            gDef.datasource("min", file.getAbsolutePath(), dsName, "MIN");
            gDef.line("min", Color.RED, "MIN");
            gDef.gprint("min", "MAX", "min:@2");
            gDef.gprint("min", "AVERAGE", "@2");
            gDef.gprint("min", "MIN", "@2@r");

        }

        RrdGraph rrdGraph = new RrdGraph(gDef);
        return rrdGraph;
    }

}

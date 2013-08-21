package xiaonei.commons.jade.example.controllers;

import net.paoding.rose.web.annotation.ReqMapping;

@ReqMapping(path = "")
public class IndexController {

    /**
     * 跳转到主页
     */
    @ReqMapping(path = "")
    public String index() {

        // 访问: webapp/views/index.jsp
        return "index";
    }
}

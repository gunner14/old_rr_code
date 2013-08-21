package com.xiaonei.commons.interceptors.http;

import java.net.URL;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 
 * {@link PostRequestProtectionInterceptor} 拦截对Web控制器的调用，
 * 对POST请求进行referer验证，防止外站提交。
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class PostRequestProtectionInterceptor extends ControllerInterceptorAdapter {

    @Override
    public Object before(Invocation inv) throws Exception {
        HttpServletRequest request = inv.getRequest();
        if ("POST".equalsIgnoreCase(request.getMethod())) {
            String referer = request.getHeader("Referer");
            if (referer != null && referer.trim().length() > 0) {
                URL url = null;
                try {
                    referer = referer.replaceAll("#", "");
                    url = new URL(referer);
                    String hosturl = url.getHost().toLowerCase();
                    if (hosturl.endsWith(OpiConstants.domainMain.toString())
                            || hosturl.endsWith(OpiConstants.domainStatic.toString())
                            || hosturl.endsWith("5q.com")) {
                        // passed filtering
                    } else {
                        return "r:" + OpiConstants.urlHome + "/Home.do";
                    }
                } catch (Exception e) {
                    // e.g: MalformedURLException
                    // refer格式有问题,这种情况理论上应该不会发生，但是后来证实在wap平台上确实发生了，所以作忽略处理
                }
            } else {
                // 没有referer的情况，也需要处理，暂时还没有处理，
                // 发生了一种情况：学校资料页，先保存到本地，再用这个页提交，虽然没有referer但是可以提交成功，这是个漏洞
            }
        }
        return true;
    }
}

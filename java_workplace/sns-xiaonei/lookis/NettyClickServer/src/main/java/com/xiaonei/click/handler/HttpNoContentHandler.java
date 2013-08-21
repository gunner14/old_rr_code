/**
 * 
 */
package com.xiaonei.click.handler;

import java.io.UnsupportedEncodingException;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelFutureListener;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipelineCoverage;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.handler.codec.http.DefaultHttpResponse;
import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.jboss.netty.handler.codec.http.HttpResponse;
import org.jboss.netty.handler.codec.http.HttpResponseStatus;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.xiaonei.click.configure.Config;
import com.xiaonei.click.handler.dispatch.RequestDispatcher;
import com.xiaonei.click.handler.http.ParamParser;

/**
 * 返回204并存储相应数据
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */

@ChannelPipelineCoverage("all")
public class HttpNoContentHandler extends SimpleChannelUpstreamHandler {

    Random r = new Random();

    private static ExecutorService es = Executors.newFixedThreadPool(16);

    @Override
    public void messageReceived(ChannelHandlerContext ctx, final MessageEvent e) throws Exception {
        es.submit(new Runnable() {

            @Override
            public void run() {
                HttpResponse response = new DefaultHttpResponse(HttpVersion.HTTP_1_1,
                        HttpResponseStatus.NO_CONTENT);
                if (r.nextInt(Integer.parseInt(Config.getInstance().get(Config.Key.DISPATCH_RATE,
                        null).toString())) == 0) {
                    HttpRequest req = (HttpRequest) e.getMessage();
                    dispatch(req);
                }
                response.addHeader(HttpHeaders.Names.SERVER, "WSEP");
                response.addHeader(HttpHeaders.Names.CONTENT_LENGTH, "0");
                response.addHeader(HttpHeaders.Names.CONNECTION, HttpHeaders.Values.CLOSE);
                ChannelFuture future = e.getChannel().write(response);
                future.addListener(ChannelFutureListener.CLOSE);
            }
        });

    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e) throws Exception {
        e.getCause().printStackTrace();
        e.getChannel().close();
    }

    public void dispatch(HttpRequest req) {
        Map<String, String> param = null;
        try {
            param = new ParamParser().parse(req);
        } catch (UnsupportedEncodingException e1) {
            e1.printStackTrace();
        }
        if (param != null && param.size() > 0) {

            RequestDispatcher.getInstance().dispatch(req.getUri(), param);
        }
    }
}

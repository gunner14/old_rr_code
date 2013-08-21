/**
 * 
 */
package com.xiaonei.click.server;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;

import com.xiaonei.click.configure.Config.Key;
import com.xiaonei.click.handler.HttpNoContentHandler;
import com.xiaonei.click.pipeline.HttpServerPipelineFactory;

/**
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class NioClickServer implements HttpServer {

    private ServerBootstrap bootstrap = null;

    @Override
    public void bind(InetSocketAddress add) {
        // Configure the server.
        bootstrap = new ServerBootstrap(new NioServerSocketChannelFactory(Executors
                .newCachedThreadPool(), Executors.newCachedThreadPool()));
        // Set up the event pipeline factory.
        bootstrap.setPipelineFactory(new HttpServerPipelineFactory(HttpNoContentHandler.class));
        // Set up the Server args.
        bootstrap.setOption("child.reuseAddress", true);
        bootstrap.setOption("reuseAddress", true);
        bootstrap.setOption("tcpNoDelay", true);
        bootstrap.bind(add);

    }

    //监听端口改变后重启服务
    @Override
    public void reloadAll() {
        if (bootstrap != null) bootstrap.releaseExternalResources();
    }

    @Override
    public void reload(Key k) {
        // TODO Auto-generated method stub

    }
}

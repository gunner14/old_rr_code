/*
 * Copyright 2009 Red Hat, Inc.
 *
 * Red Hat licenses this file to you under the Apache License, version 2.0
 * (the "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package org.jboss.netty.example.http.snoop.lwb;

import java.util.Set;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelFutureListener;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipelineCoverage;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.handler.codec.http.Cookie;
import org.jboss.netty.handler.codec.http.CookieDecoder;
import org.jboss.netty.handler.codec.http.CookieEncoder;
import org.jboss.netty.handler.codec.http.DefaultHttpResponse;
import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.jboss.netty.handler.codec.http.HttpResponse;
import org.jboss.netty.handler.codec.http.HttpResponseStatus;
import org.jboss.netty.handler.codec.http.HttpVersion;

/**
 * 
 * @author 李伟博
 */
@ChannelPipelineCoverage("all")
public class MyHttpRequestHandler extends SimpleChannelUpstreamHandler {

    //private volatile HttpRequest request;
    //private volatile boolean readingChunks;
    //private final StringBuilder responseContent = new StringBuilder();

    public static final String CRLF = "\r\n";

    @Override
    public void messageReceived(ChannelHandlerContext ctx, MessageEvent e) throws Exception {

        boolean readingChunks = false;
        StringBuilder responseContent = new StringBuilder(1024);

        if (!readingChunks) {
            HttpRequest request = (HttpRequest) e.getMessage();

            String uri = request.getUri();
            String path;
            if (uri.indexOf('?') != -1) {
                path = uri.substring(0, uri.indexOf('?'));
            } else {
                path = uri;
            }

            //remove trailing slash
            if (path.length() > 0 && path.charAt(path.length() - 1) == '/') {
                path = path.substring(0, path.length() - 1);
            }
            IReqHandler handler = RequestDispather.getInstance().dispatch(path);
            if (handler != null) {
                String ret = handler.handle(request);
                responseContent.append(ret);
                writeResponse(e, responseContent, request);
            } else {
                System.err.println("Unhandled uri:" + uri);
            }
            e.getChannel().close();
            /*HttpVersion version = request.getProtocolVersion();
            
            responseContent.append(method);
            responseContent.append(' ');
            responseContent.append(uri);
            responseContent.append(' ');
            responseContent.append(version.toString());
            responseContent.append(CRLF);
            
            Set<String> headerNames = request.getHeaderNames();
            for (String headerName : headerNames) {
            	List<String> headers = request.getHeaders(headerName);
            	for (String headerValue : headers) {
            		responseContent.append(headerName);
            		responseContent.append(": ");
            		responseContent.append(headerValue);
            		responseContent.append(CRLF);
            	}
            }

            if (request.isChunked()) {
            	readingChunks = true;
            } else {
            	ChannelBuffer content = request.getContent();
            	if (content.readable()) {
            		responseContent.append(CRLF);
            		responseContent.append(content.toString("UTF-8"));
            		responseContent.append(CRLF);
            	}
            	writeResponse(e);
            }*/
        } else {
            throw new UnsupportedOperationException();
            /*HttpChunk chunk = (HttpChunk) e.getMessage();
            if (chunk.isLast()) {
            	readingChunks = false;
            	responseContent.append("END OF CONTENT\r\n");
            	writeResponse(e);
            } else {
            	responseContent.append("CHUNK: "
            			+ chunk.getContent().toString("UTF-8") + "\r\n");
            }*/
        }
    }

    private void writeResponse(MessageEvent e, StringBuilder responseContent, HttpRequest request) {
        // Convert the response content to a ChannelBuffer.
        ChannelBuffer buf = ChannelBuffers.copiedBuffer(responseContent.toString(), "UTF-8");
        responseContent.setLength(0);

        // Decide whether to close the connection or not.
        boolean close = HttpHeaders.Values.CLOSE.equalsIgnoreCase(request
                .getHeader(HttpHeaders.Names.CONNECTION))
                || request.getProtocolVersion().equals(HttpVersion.HTTP_1_0)
                && !HttpHeaders.Values.KEEP_ALIVE.equalsIgnoreCase(request
                        .getHeader(HttpHeaders.Names.CONNECTION));

        // Build the response object.
        HttpResponse response = new DefaultHttpResponse(HttpVersion.HTTP_1_1, HttpResponseStatus.OK);
        response.setContent(buf);
        response.setHeader(HttpHeaders.Names.CONTENT_TYPE, "text/plain; charset=UTF-8");

        if (!close) {
            // There's no need to add 'Content-Length' header
            // if this is the last response.
            response.setHeader(HttpHeaders.Names.CONTENT_LENGTH, String
                    .valueOf(buf.readableBytes()));
        }

        String cookieString = request.getHeader(HttpHeaders.Names.COOKIE);
        if (cookieString != null) {
            CookieDecoder cookieDecoder = new CookieDecoder();
            Set<Cookie> cookies = cookieDecoder.decode(cookieString);
            if (!cookies.isEmpty()) {
                // Reset the cookies if necessary.
                CookieEncoder cookieEncoder = new CookieEncoder(true);
                for (Cookie cookie : cookies) {
                    cookieEncoder.addCookie(cookie);
                }
                response.addHeader(HttpHeaders.Names.SET_COOKIE, cookieEncoder.encode());
            }
        }

        // Write the response.
        ChannelFuture future = e.getChannel().write(response);

        // Close the connection after the write operation is done if necessary.
        if (close) {
            future.addListener(ChannelFutureListener.CLOSE);
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e) throws Exception {
        e.getCause().printStackTrace();
        e.getChannel().close();
    }
}

package com.renren.xcs;

public class NodeUtils {

    public static String constructNodeName(String host, int port) {
        return new StringBuffer().append(host).append(":").append(String.valueOf(port)).toString();
    }
    
    public static NodeName parseNodeName(String nodeName){
        String[] nodePart = nodeName.split(":", 2);
        return new NodeName(nodePart[0], Integer.parseInt(nodePart[1]));
    }
    
    public static class NodeName{
        private String host;
        private int port;
        
        public String getHost() {
            return host;
        }
        
        public void setHost(String host) {
            this.host = host;
        }
        
        public int getPort() {
            return port;
        }
        
        public void setPort(int port) {
            this.port = port;
        }

        public NodeName(String host, int port) {
            super();
            this.host = host;
            this.port = port;
        }
        
    }
}

package com.renren.xoa.test.disconnect;

import com.renren.xoa.XoaClient;


public class ClientDisconnectTest {

    public static void main(String[] args) {
        
        String serviceId = "test.xoa.renren.com";
        XoaClient client = new XoaClient();
        client.ping(serviceId);
        client.ping("entries.xoa.renren.com");
        while (true) {
            
            try {
                Thread.sleep(10000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            client.ping(serviceId);
        }
        
        /*synchronized (client) {
            try {
                client.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }*/
        
    }
    
}

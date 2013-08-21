package com.renren.xoa.test.lite;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;
import com.renren.xoa.test.api.TestService;


public class ThroughputControlTest {

    public static void main(String[] args) {
        
        final TestService service = ServiceFactories.getFactory().getService(TestService.class);
        
        ExecutorService exe = Executors.newFixedThreadPool(30);
        while (true) {
            
            exe.submit(new Runnable() {
                
                @Override
                public void run() {
                    try {
                        ServiceFuture<String> future = service.getText(1, 1000);
                        String s = ServiceFutureHelper.executeWithThroughputControl(future, "text", 4, 5000);
                        System.out.println(s);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        
    }
    
}

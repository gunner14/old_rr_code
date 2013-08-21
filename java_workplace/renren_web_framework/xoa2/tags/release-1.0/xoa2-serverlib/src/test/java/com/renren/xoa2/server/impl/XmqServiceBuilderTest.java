package com.renren.xoa2.server.impl;

import java.io.File;
import java.util.List;

import junit.framework.Assert;

import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TProtocol;
import org.junit.Test;

import com.renren.xoa2.server.conf.Configuration;
import com.renren.xoa2.server.conf.Configuration.ConfigurationFactory;

public class XmqServiceBuilderTest {
    private String confFile = ".\\src\\test\\resource\\xmq-ping.xml";

    @Test
    public void testBuild() {
        String localConf = confFile.replace("\\", File.separator);
        
        Configuration conf = ConfigurationFactory.newConfiguration(localConf);

        XmqServiceBuilder serviceBuilder = new XmqServiceBuilder();
        List<XoaServiceConfig> configList = conf.listServiceConfig();


        XoaService service = serviceBuilder.build(configList.get(0));
        TProcessor processor = service.getProcessor();
        
        Assert.assertEquals(processor.getClass().getName(),
                "com.renren.xoa2.server.processor.XoaProcessor");
        
        service.getServer().stop();
    }
}

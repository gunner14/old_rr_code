package com.xiaonei.jade.dbplugin.listener;

import java.util.Arrays;
import java.util.Collection;


import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import com.xiaonei.jade.dbplugin.configuration.ConfigJudge;
import com.xiaonei.jade.dbplugin.listener.impl.DataMonitorListenerImpl;
import com.xiaonei.jade.dbplugin.model.DataModel;
import com.xiaonei.jade.dbplugin.writer.DataMonitorWriter;

/**
 * IDataMonitorListenerTest <br>
 * 
 * @see DataMonitorListener
 * 
 * @author tai.wang@opi-corp.com Jul 16, 2010 - 3:17:46 PM
 */
@RunWith(Parameterized.class)
public class IDataMonitorListenerTest {

    private DataMonitorListener test;

    private ConfigJudge configJudgeMock;

    private DataMonitorWriter writerMock;

    private DataModel data;

    @Parameters
    public static Collection<?> prepareData() {
        DataModel dataModel = new DataModel();
        dataModel.setClassName("com.Test.method");
        dataModel.setClientIp("127.0.0.1");
        dataModel.setCostTime(100);
        dataModel.setDataSource("dataSource");
        dataModel.setSql("select sql from datamodel where spring = ?");
        dataModel.setSqlParams("true");
        dataModel.setStartTime(System.currentTimeMillis());
        Object[][] data = { { dataModel } };
        return Arrays.asList(data);
    }

    public IDataMonitorListenerTest(DataModel data) {
        this.data = data;
    }

    @Before
    public void setUp() throws Exception {
        configJudgeMock = new ConfigJudge() {

            @Override
            public boolean isAllowed(DataModel data) {
                return true;
            }
        };
        this.writerMock = new DataMonitorWriter() {

            @Override
            public void write(String content) {
                System.out.println("writer is printing, the content is \"" + content + "\"");
            }
        };
        test = new DataMonitorListenerImpl();
        test.setConfigJudge(configJudgeMock);
        test.setWriter(writerMock);
    }

    @After
    public void tearDown() throws Exception {
    }

    @Test
    public final void testListen() {
        test.listen(data);
    }

}

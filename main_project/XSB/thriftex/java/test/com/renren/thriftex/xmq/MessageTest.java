package com.renren.thriftex.xmq;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class MessageTest {

    @BeforeClass
    public static void setUpBeforeClass() throws Exception {
    }

    @AfterClass
    public static void tearDownAfterClass() throws Exception {
    }

    @Before
    public void setUp() throws Exception {
    }

    @After
    public void tearDown() throws Exception {
    }

    @Test
    public void testPackMessage() {
        Message message = new Message();
        message.opCode = Constant.CONSUMER_DATA;
        message.seqID = 10000000000L;
        message.data = new byte[255];
        
        for (int i = 0; i < 255; i++) {
            message.data[i] = (byte)i;
        }
        
        byte[] result = Message.packMessage(message.opCode, message.seqID, 
                message.data, message.data.length);
        
        Message messageX = Message.unpackMessage(result);
        
        assertEquals(message.opCode, messageX.opCode);
        assertEquals(message.seqID, messageX.seqID);
        assertArrayEquals(message.data, messageX.data);
    }

}

package com.renren.xoa.commons.spdy.frame.control;

import java.util.Arrays;
import java.util.Map;

import com.renren.xoa.commons.spdy.Utils;
import com.renren.xoa.commons.spdy.datastructure.NameValueBlock;

/**
 * <pre>
 *   +----------------------------------+
 *   |1|        1        |        2     |
 *   +----------------------------------+
 *   | Flags (8)  |  Length (24 bits)   |
 *   +----------------------------------+
 *   |X|          Stream-ID (31bits)    |
 *   +----------------------------------+
 *   | Unused        |    NV entries    |
 *   +----------------------------------|
 *   |     Name/value header block      |
 *   |              ...                 |
 * </pre>
 * 
 * @author Lookis (lookisliu@gmail.com)
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public class DefaultSynReply extends SynReply {

	private static final long serialVersionUID = 1L;

    //private int unused;

    private NameValueBlock nameValueEntries;

    //    public SynReply(byte flags, int streamID) {
    //        super(ControlFrame.ControlFrameType.SYN_REPLY, flags);
    //        Utils.checkLength(streamID, 31);
    //        this.streamID = streamID;
    //    }

    DefaultSynReply(byte flags, byte[] dataInByte) {
        super(flags, dataInByte);
    }

    DefaultSynReply(byte flags, int streamId) {
        super(flags, null);
        setStreamId(streamId);
        nameValueEntries = new NameValueBlock();
    }

    public String putNV(String name, String value) {
        return nameValueEntries.put(name, value);
    }

    public Map<String, String> getNVMap() {
        return nameValueEntries;
    }

    @Override
    public byte[] getData() {
        byte[] idByte = Utils.intToByte(streamId);
        //这里忽略了NV前的unused字段
        byte[] nvEntriesCountInByte = Utils.intToByte(nameValueEntries.size());
        byte[] nvByte = nameValueEntries.toByteStructure();
        byte[] data = Utils.mergeBytes(idByte, nvEntriesCountInByte, nvByte);

        return data;
    }
    
    
    //TODO 实现getLength方法
    /*@Override
    public int getLength() {
    	return 0;
    }*/

    @Override
    protected void buildDataFromByte(byte[] dataInByte) {
        if (dataInByte == null || dataInByte.length == 0) {
            //采用第二种构造方式，自己setXXX
            return;
        }
        //streamId部分
        this.streamId = Utils.streamId(dataInByte);
        //unused,NVCount部分,无用
        //byte[] unusedAndNVCount = Utils.copyBytesFromHighPos(dataInByte, 4, 8);
        //NV部分
        byte[] nvData = Arrays.copyOfRange(dataInByte, 8, dataInByte.length);
        this.nameValueEntries = new NameValueBlock(nvData);
    }
    
}

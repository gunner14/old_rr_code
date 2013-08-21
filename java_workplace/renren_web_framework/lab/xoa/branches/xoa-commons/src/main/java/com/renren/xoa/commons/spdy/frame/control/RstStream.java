/**
 * 
 */
package com.renren.xoa.commons.spdy.frame.control;

import java.util.Arrays;

import com.renren.xoa.commons.spdy.Utils;
import com.renren.xoa.commons.spdy.frame.ControlFrame;
import com.renren.xoa.commons.spdy.frame.DefaultControlFrame;

/**
 * <pre>
 *   +-------------------------------+
 *   |1|       1        |      3     |
 *   +-------------------------------+
 *   | Flags (8)  |         8        |
 *   +-------------------------------+
 *   |X|          Stream-ID (31bits) |
 *   +-------------------------------+
 *   |          Status code          | 
 *   +-------------------------------+
 * </pre>
 * 
 * @author Lookis (lookisliu@gmail.com)
 * 
 */
public class RstStream extends DefaultControlFrame {

    /**
     * 
     */
    private static final long serialVersionUID = 2323841155922744046L;

    final static StatusCode[] values = new StatusCode[10];

    public static enum StatusCode {
        UNKNOW_ERROR(0), PROTOCOL_ERROR(1), INVALID_STREAM(2), REFUSED_STREAM(3), UNSUPPORTED_VERSION(4), CANCEL(
                5), INTERNAL_ERROR(6);

        private int value;

        StatusCode(int value) {
            this.value = value;
            RstStream.values[value] = this;
        }

        public int getValue() {
            return value;
        }

    }

    private StatusCode statusCode;

    private int streamID;

    public RstStream(byte flags, byte[] dataInByte) {
        super(ControlFrame.ControlFrameType.RST_STREAM, flags, dataInByte);
    }

    public static long getSerialversionuid() {
        return serialVersionUID;
    }

    public StatusCode getStatusCode() {
        return statusCode;
    }

    public int getStreamID() {
        return streamID;
    }

    @Override
    public byte[] getData() {
        //这里直接忽略了StreamID前的保留位
        //TODO：做好处理
        byte[] idByte = Utils.intToByte(streamID);
        byte[] codeByte = Utils.intToByte(statusCode.getValue());
        byte[] data = Utils.mergeBytes(idByte, codeByte);
        return data;
    }

    @Override
    protected void buildDataFromByte(byte[] dataInByte) {
        //fix X bit
        this.streamID = Utils.streamId(dataInByte);
        this.statusCode = RstStream.values[Utils.bytesToInt(dataInByte, 4)];
    }
}

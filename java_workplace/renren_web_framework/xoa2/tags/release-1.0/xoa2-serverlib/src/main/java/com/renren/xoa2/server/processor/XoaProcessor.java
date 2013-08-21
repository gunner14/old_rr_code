package com.renren.xoa2.server.processor;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.nio.ByteBuffer;

import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TField;
import org.apache.thrift.protocol.TList;
import org.apache.thrift.protocol.TMap;
import org.apache.thrift.protocol.TMessage;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.protocol.TSet;
import org.apache.thrift.protocol.TStruct;
import org.apache.thrift.protocol.TType;
import org.apache.thrift.transport.TMemoryBuffer;
import org.apache.thrift.transport.TTransport;
import org.perf4j.StopWatch;
import org.perf4j.log4j.Log4JStopWatch;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.server.impl.XoaService;

public class XoaProcessor implements TProcessor {
    private TProcessor realProcessor;
    
    private XoaService xoaService_;
    private static Logger logger = LoggerFactory.getLogger(XoaProcessor.class);

    
    public XoaProcessor(TProcessor realProcessor) {
        this.realProcessor = realProcessor;
    }

    public void setXoaService(XoaService xoaService) {
        xoaService_ = xoaService;
    }
    
    public static void cloneProto(TProtocol inProt, TProtocol outProt,
            byte type, int maxDepth) throws TException {
        if (maxDepth <= 0) {
            throw new TException("Maximum skip depth exceeded");
        }

        switch (type) {
        case TType.BOOL:
            boolean blv = inProt.readBool();
            outProt.writeBool(blv);
            break;

        case TType.BYTE:
            byte btv = inProt.readByte();
            outProt.writeByte(btv);
            break;

        case TType.I16:
            short stv = inProt.readI16();
            outProt.writeI16(stv);
            break;

        case TType.I32:
            int iv = inProt.readI32();
            outProt.writeI32(iv);
            break;

        case TType.I64:
            long lv = inProt.readI64();
            outProt.writeI64(lv);
            break;

        case TType.DOUBLE:
            double dv = inProt.readDouble();
            outProt.writeDouble(dv);
            break;

        case TType.STRING:
            ByteBuffer bbv = inProt.readBinary();
            outProt.writeBinary(bbv);
            break;

        case TType.STRUCT:
            TStruct struct = inProt.readStructBegin();
            outProt.writeStructBegin(struct);
            while (true) {
                TField field = inProt.readFieldBegin();
                if (field.type == TType.STOP) {
                    outProt.writeFieldStop();
                    break;
                }
                outProt.writeFieldBegin(field);
                cloneProto(inProt, outProt, field.type, maxDepth - 1);
                inProt.readFieldEnd();
                outProt.writeFieldEnd();
            }
            inProt.readStructEnd();
            outProt.writeStructEnd();
            break;

        case TType.MAP:
            TMap map = inProt.readMapBegin();
            outProt.writeMapBegin(map);
            for (int i = 0; i < map.size; i++) {
                cloneProto(inProt, outProt, map.keyType, maxDepth - 1);
                cloneProto(inProt, outProt, map.valueType, maxDepth - 1);
            }
            inProt.readMapEnd();
            outProt.writeMapEnd();
            break;

        case TType.SET:
            TSet set = inProt.readSetBegin();
            outProt.writeSetBegin(set);
            for (int i = 0; i < set.size; i++) {
                cloneProto(inProt, outProt, set.elemType, maxDepth - 1);
            }
            inProt.readSetEnd();
            outProt.writeSetEnd();
            break;

        case TType.LIST:
            TList list = inProt.readListBegin();
            outProt.writeListBegin(list);
            for (int i = 0; i < list.size; i++) {
                cloneProto(inProt, outProt, list.elemType, maxDepth - 1);
            }
            inProt.readListEnd();
            outProt.writeListEnd();
            break;

        default:
            break;
        }
    }

    TProtocol createBufferProtocol(TProtocol in) throws TException {
        Class<?> inProtoClass = in.getClass();

        TProtocol bufProtocol = null;

        try {
            Constructor<?> ctor = inProtoClass
                    .getConstructor(new Class[] { TTransport.class });
            bufProtocol = (TProtocol) ctor.newInstance(new TMemoryBuffer(256));
        } catch (SecurityException e) {
            logger.warn("SecurityException: " + e);
        } catch (NoSuchMethodException e) {
            logger.warn("NoSuchMethodException: " + e);
        } catch (InstantiationException e) {
            logger.warn("InstantiationException: " + e);
        } catch (IllegalAccessException e) {
            logger.warn("IllegalAccessException: " + e);
        } catch (IllegalArgumentException e) {
            logger.warn("IllegalArgumentException: " + e);
        } catch (InvocationTargetException e) {
            logger.warn("InvocationTargetException: " + e);
        }

        return bufProtocol;
    }
   

    @Override
    public boolean process(TProtocol in, TProtocol out) throws TException {
        StopWatch stopWatch = new Log4JStopWatch();

        TProtocol bufProtocol = createBufferProtocol(in);

        TMessage msg = in.readMessageBegin();

        bufProtocol.writeMessageBegin(msg);
        cloneProto(in, bufProtocol, TType.STRUCT, 5);
        bufProtocol.writeMessageEnd();
        boolean res = realProcessor.process(bufProtocol, out);
        
        String desc = in.getTransport().getDescription();
        StringBuffer key = new StringBuffer();
        key.append('.').append(msg.name);
        long elapsed = stopWatch.getElapsedTime();
        if (stopWatch.getElapsedTime() > 100L) {
            key.append(".SLOW");
            logger.warn(desc + " called " + key + " time=" + elapsed);
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug(desc + " called " + key + " time=" + elapsed);
            }
        }
  
        xoaService_.setCallMethodInfo(msg.name, (int)elapsed);
        stopWatch.stop(key.toString());

        return res;
    }
    
}

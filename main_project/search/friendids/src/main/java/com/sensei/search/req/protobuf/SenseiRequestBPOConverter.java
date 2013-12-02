package com.sensei.search.req.protobuf;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import org.apache.log4j.Logger;

import com.google.protobuf.ByteString;
import com.xiaonei.xce.fof.service.impl.FriendOfFriendRequest;
import com.xiaonei.xce.fof.service.impl.FriendOfFriendResult;

public class SenseiRequestBPOConverter {

	private static Logger logger = Logger.getLogger(SenseiRequestBPOConverter.class);
	
  public static FriendOfFriendRequest convert(SenseiRequestBPO.Request req)
  {
    try
    {
      ByteString value = req.getVal();
      byte[] raw = value.toByteArray();
      ByteArrayInputStream bais = new ByteArrayInputStream(raw);
      ObjectInputStream ois = new ObjectInputStream(bais);
      FriendOfFriendRequest ret = (FriendOfFriendRequest) ois.readObject();
      return ret;
    } catch (Exception e)
    {
      logger.error("serialize request", e);
    }
    return null;
  }
  public static SenseiRequestBPO.Request convert(FriendOfFriendRequest req)
  {
    SenseiRequestBPO.Request.Builder builder = SenseiRequestBPO.Request.newBuilder();
    try
    {
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      ObjectOutputStream oos;
      oos = new ObjectOutputStream(baos);
      oos.writeObject(req);
      oos.close();
      byte[] raw = baos.toByteArray();
      builder.setVal(ByteString.copyFrom(raw));
      return builder.build();
    } catch (IOException e)
    {
      logger.error("deserialize request", e);
    }
    return SenseiRequestBPO.Request.getDefaultInstance();
  }
  public static FriendOfFriendResult convert(SenseiResultBPO.Result req)
  {
    try
    {
      ByteString value = req.getVal();
      byte[] raw = value.toByteArray();
      ByteArrayInputStream bais = new ByteArrayInputStream(raw);
      ObjectInputStream ois = new ObjectInputStream(bais);
      FriendOfFriendResult ret = (FriendOfFriendResult) ois.readObject();
      return ret;
    } catch (Exception e)
    {
      logger.error("serialize result", e);
    }
    return null;
  }
  public static SenseiResultBPO.Result convert(FriendOfFriendResult req)
  {
    SenseiResultBPO.Result.Builder builder = SenseiResultBPO.Result.newBuilder();
    try
    {
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      ObjectOutputStream oos;
      oos = new ObjectOutputStream(baos);
      oos.writeObject(req);
      oos.close();
      byte[] raw = baos.toByteArray();
      builder.setVal(ByteString.copyFrom(raw));
      return builder.build();
    } catch (IOException e)
    {
      logger.error("deserialize result", e);
    }
    return SenseiResultBPO.Result.getDefaultInstance();
  }
}

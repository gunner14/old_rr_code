package com.xiaonei.xce.userurl;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import xce.tripod.TripodClient;
import xce.tripod.TripodException;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.tripod.proto.UserBaseProto.PbUserUrl;
import com.xiaonei.xce.log.Oce;
import com.xiaonei.xce.log.TimeCost;

public final class UserUrlTripodAdapter<T extends UserUrlInfo> {

    private static String NAMESPACE_ID = "xce_user";

    private static String BIZ_ID = "user_url";

    private UserUrlFactory<T> _factory;

    private TripodClient _client;

    public UserUrlTripodAdapter(UserUrlFactory<T> factory) {
        _factory = factory;
        _client = new TripodClient(NAMESPACE_ID, BIZ_ID);
    }

    public T getUserUrl(int id) {
        TimeCost cost = TimeCost.begin("UserUrlTripodAdapter.getUserUrl id: " + id);
        T result = _factory.create();
        if (id <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = _client.get(String.valueOf(id), 0);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUserUrl[ tripod ] id=" + id
                            + " TripodExcpetion msg:" + e.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length > 0) {
                PbUserUrl data = PbUserUrl.parseFrom(dataRaw);
                if (data.getId() != 0) {
                    result.parse(data);
                } else {
                    result = null;
                }
            }
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(this.getClass().getName() + //NL
                    ".getUserUrl[ tripod ] id=" + id + " Ice.TimeoutException");
            throw e;
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(this.getClass().getName() + //NL
                    ".getUserUrl id=" + id + " InvalidProtocolBuf");
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public Map<Integer, T> multiGetUserUrl(List<Integer> ids) {
        TimeCost cost = TimeCost.begin("UserUrlTripodAdapter.multiGetUserUrl id: " + ids);
        Map<Integer, T> result = null;
        try {
            List<String> strKeys = new LinkedList<String>();
            for (Integer key : ids) {
                strKeys.add(String.valueOf(key));
            }
            Map<String, byte[]> mapValue = _client.multiGet(strKeys, 0);
            result = new HashMap<Integer, T>();
            for (Map.Entry<String, byte[]> m : mapValue.entrySet()) {
                if (m.getValue() != null && m.getValue().length > 0) {
                    try {
                        PbUserUrl pbdata = PbUserUrl.parseFrom(m.getValue());
                        T data = _factory.create();
                        data.parse(pbdata);
                        if (data.getId() == Integer.parseInt(m.getKey())) {
                            result.put(Integer.parseInt(m.getKey()), data);
                        }
                    } catch (InvalidProtocolBufferException e) {
                        Oce.getLogger().error(this.getClass().getName() + //NL
                                ".multiGetUserUrl id=" + m.getKey() + " InvalidProtocolBuf");
                    }
                }
            }
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(this.getClass().getName() + //NL
                    ".getUserUrl[ tripod ] idcount=" + ids.size() + " Ice.TimeoutException");
            throw e;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            cost.endFinally();
        }
        return result;
    }

    boolean loadUserUrl(int id) {
        TimeCost cost = TimeCost.begin("UserUrlTripodAdapter.loadUserUrl id: " + id);
        boolean result = false;
        try {
            result = _client.load(String.valueOf(id), 0, false);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(this.getClass().getName() + // NL
                    ".loadUserUrl id=" + id + " exeption:" + e.toString());
            return false;
        } catch (Exception e) {
            Oce.getLogger().error(this.getClass().getName() + // NL
                    ".loadUserUrl id=" + id + " exeption:" + e.toString());
            return false;
        } finally {
            cost.endFinally();
        }
        return result;
    }

}

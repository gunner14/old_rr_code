package com.xiaonei.xce.accountconnect;

import Ice.TimeoutException;
import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.tripod.proto.AccountConnectProto.PbAccountConnect;
import com.xiaonei.xce.log.Oce;
import xce.account.AccountConnectPrx;
import xce.account.AccountConnectPrxHelper;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.tripod.TripodClient;
import xce.tripod.TripodException;

public final class AccountConnectAdapter extends ReplicatedClusterAdapter {

    private static final String ctr_endpoints = "ControllerAccountConnect";

    private static final int _interval = 120;

    private final static int EXPIRE_TIME = 3600 * 24 * 30;

    private final static String NAMESPACE_ID = "xce_user";

    private final static String BIZ_ID = "account_connect";

    private static AccountConnectAdapter _instance = null;

    private final TripodClient _client;

    public static AccountConnectAdapter getInstance() {
        if (AccountConnectAdapter._instance == null) {
            synchronized (AccountConnectAdapter.class) {
                if (AccountConnectAdapter._instance == null) {
                    AccountConnectAdapter._instance = new AccountConnectAdapter();
                }
            }
        }
        return AccountConnectAdapter._instance;
    }

    private AccountConnectAdapter() {
        super(ctr_endpoints, _interval);
        _client = new TripodClient(NAMESPACE_ID, BIZ_ID);
    }

    private AccountConnectPrx getAccountConnectPrx(int userId) {
        Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
        if (prx0 != null) {
            return AccountConnectPrxHelper.uncheckedCast(prx0.ice_timeout(300).ice_twoway());
        } else {
            return null;
        }
    }

    /**
     * 查询连接的人人ID
     *
     * @param cid  连接ID
     * @param type 连接类型
     * @return 查找成功返回人人ID，失败返回-1
     * @throws Ice.TimeoutException
     */
    public long getUid(String cid, int type) {
        long result = -1;
        byte[] dataRaw = null;
        String key = String.valueOf(type) + "_" + cid;
        try {
            dataRaw = _client.get(key, EXPIRE_TIME);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(this.getClass().getName() + ".getUid cid= " + cid
                    + " type= " + type + " Ice.TimeoutException");
            throw e;
        } catch (TripodException e) {
            Oce.getLogger().error(this.getClass().getName() + ".getUid[ tripod ] cid= " + cid
                    + " type= " + type + " TripodException msg:" + e.getMessage());
        }

        if (dataRaw != null && dataRaw.length > 0) {
            try {
                PbAccountConnect data = PbAccountConnect.parseFrom(dataRaw);
                result = data.getUid();
            } catch (InvalidProtocolBufferException e) {
                Oce.getLogger().error(this.getClass().getName() + ".getUid[ tripod ] cid= " + cid
                        + " type= " + type + " InvalidProtocolBuf");
            }
        }

        return result;
    }

    /**
     * 创建或修改连接关系
     *
     * @param cid  连接ID
     * @param type 连接类型
     * @param uid  人人ID
     * @return 是否成功
     * @throws Ice.TimeoutException
     */
    public boolean set(String cid, int type, long uid) {
        try {
            AccountConnectPrx prx = getAccountConnectPrx((int) uid);
            if(prx == null) {
                Oce.getLogger().error(this.getClass().getName() + ".set cid= " + cid
                        + " type= " + type + " uid= " + uid +  " prx is null");
                return false;
            } else {
                return prx.set(cid, type, uid);
            }
        } catch (TimeoutException e) {
            Oce.getLogger().error(this.getClass().getName() + ".set cid= " + cid
                    + " type= " + type + " uid= " + uid +  " Ice.TimeoutException");
            throw e;
        }
    }

    public static void main(String[] args) {
        String cid = "joyeru@qq.com";
        long uid = 35178;
        boolean result = AccountConnectAdapter.getInstance().set(cid, 0x01, uid);
        long uidGet = AccountConnectAdapter.getInstance().getUid(cid, 0x01);
        System.out.println("set:" + result + " getUid:" + uidGet);
        System.exit(0);
    }
}

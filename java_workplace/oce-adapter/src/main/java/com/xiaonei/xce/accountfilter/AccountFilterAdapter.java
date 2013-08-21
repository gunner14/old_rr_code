package com.xiaonei.xce.accountfilter;

import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.log.Oce;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.account.AccountFilterPrx;
import xce.account.AccountFilterPrxHelper;

public final class AccountFilterAdapter extends ReplicatedClusterAdapter {

    private static final String ctr_endpoints = "ControllerAccountFilter";

    private static int _interval = 120;
    
    private static AccountFilterAdapter _instance = null;

    public static AccountFilterAdapter getInstance() {
        if (AccountFilterAdapter._instance == null) {
            synchronized (AccountFilterAdapter.class) {
                if (AccountFilterAdapter._instance == null) {
                    AccountFilterAdapter._instance = new AccountFilterAdapter();
                }
            }
        }
        return AccountFilterAdapter._instance;
    }
    
    private AccountFilterAdapter() {
        super(ctr_endpoints, _interval);
    }

    private AccountFilterPrx getAccountFilterPrx(int userId) {
        Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
        if (prx0 != null) {
            AccountFilterPrx prx = AccountFilterPrxHelper.uncheckedCast(prx0.ice_timeout(300)
                    .ice_twoway());
            return prx;
        } else {
            return null;
        }
    }

    /**
     * @description 通过SHA256串过滤人人帐号
     * @param 邮箱的SHA256摘要列表。传入List大小最好不要超过1000，防止超时
     * @return 在列表中出现的人人id。如果有一个邮箱对应多个id，多个id都返回
     */
    public List<Integer> getIdBySHA256(List<String> digests) {
        AccountFilterPrx prx = null;
        List<Integer> result = new ArrayList<Integer>();
        try {
            prx = getAccountFilterPrx(0);
            String[] digestArray = new String[digests.size()];
            digests.toArray(digestArray);
            int[] ids = prx.GetIdBySHA256(digestArray);
            for (int id : ids) {
                result.add(id);
            }
        } catch (Exception e) {
            Oce.getLogger().error(this.getClass().getName() + ".getIdBySHA256 [" + prx //NL
                    + "] digests.size= " + digests.size() + " " + e.toString());
        };
        return result;
    }

    public static void main(String[] args) {
        try {
            ArrayList<String> digests = new ArrayList<String>();
            for(int i = 10000; i < 20000; i++) { //测试效率
                digests.add("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20" + i);
            }
            digests.add("35f0f4b3ed7282abdd4557da983673757260ba015cc641f31e0a85721bc53b32");//personalway@gmail.com => 21578/713236094
            digests.add("32F6A91FBF29A6FF876E97629992F9B5645CA2BFB36CC2A02B6D283B3B24618A");//cshust@smail.hust.edu.cn => 35178
            List<Integer> result = AccountFilterAdapter.getInstance().getIdBySHA256(digests);
            for (int id : result) {
                System.out.println("hit id " + id);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.exit(0);
    }
}

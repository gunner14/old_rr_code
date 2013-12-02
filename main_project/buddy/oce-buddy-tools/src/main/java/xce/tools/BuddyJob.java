package xce.tools;

import java.util.List;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.BuddyApplyWithInfo;

import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheAdapterN;
import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheDataN;
import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheFactoryN;
import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheWithTotalN;

public final class BuddyJob {
  public static void main(String[] args) {
    try {
      int[] _accepter =
          {224674572, 287285057, 287286115, 287286312, 287510926, 287284111, 287286920, 287285924,
              287286782, 287284388, 287286452, 287394429, 287516742, 287519876, 287285254,
              287508269, 236829155, 321257651, 321475480, 322066917, 322850713, 322852523,
              318868322, 339527319, 347730747, 345684001};

      BuddyCoreAdapterImpl st = new BuddyCoreAdapterImpl();
      BuddyApplyCacheAdapterN<MyBuddyApplyCacheDataN, MyBuddyApplyCacheWithTotalN> _cacheAdapter =
          new BuddyApplyCacheAdapterN<MyBuddyApplyCacheDataN, MyBuddyApplyCacheWithTotalN>(
              new MyBuddyApplyCacheFactoryN());
      // _cacheAdapter.reload(_accepter);
      for (int j : _accepter) {
        // while(true){
        List<MyBuddyApplyCacheDataN> _cacheData2 = _cacheAdapter.getBuddyApplyCache(j, 0, 100);
        // if(_cacheData2.size()==0){
        // break;
        // }

        for (int i = 0; i < _cacheData2.size(); i++) {
          MyBuddyApplyCacheDataN tmp = _cacheData2.get(i);
          System.out.println(tmp.getApplicant() + " " + j);
          BuddyApplyWithInfo request = new BuddyApplyWithInfo(tmp.getApplicant(), j);
          st.acceptApply(request);
          Thread.sleep(500);
        }
        // }
      }
      System.out.println("done");
      System.exit(0);

    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}


class MyBuddyApplyCacheDataN extends BuddyApplyCacheDataN {

}


class MyBuddyApplyCacheWithTotalN extends BuddyApplyCacheWithTotalN<MyBuddyApplyCacheDataN> {

}


class MyBuddyApplyCacheFactoryN
    implements
      BuddyApplyCacheFactoryN<MyBuddyApplyCacheDataN, MyBuddyApplyCacheWithTotalN> {

  public MyBuddyApplyCacheDataN createBuddyApplyCacheDataN() {
    // TODO Auto-generated method stub
    return new MyBuddyApplyCacheDataN();
  }

  public MyBuddyApplyCacheWithTotalN createBuddyApplyCacheWithTotalN() {
    // TODO Auto-generated method stub
    return new MyBuddyApplyCacheWithTotalN();
  }

}

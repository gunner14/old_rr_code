package xce.tools;

import org.apache.log4j.BasicConfigurator;

import xce.buddy.BuddyRelationCacheManagerPrx;
import xce.buddy.BuddyRelationCacheManagerPrxHelper;
import xce.buddy.BuddyRelationLoaderPrx;
import xce.buddy.BuddyRelationLoaderPrxHelper;
import xce.buddy.Relationship;
import xce.util.channel.Channel;

import com.xiaonei.xce.buddyadapter.buddyrelationcache.BuddyRelationTripodCacheAdapter;
import com.xiaonei.xce.domain.buddy.BuddyRelation;

public class BuddyRelationStat {

  static void reload(int host, int guest) {
    try {
      Ice.ObjectPrx prx =
          Channel.newBuddyChannel("").getCommunicator().stringToProxy("L@BuddyRelationLoader0");
      BuddyRelationLoaderPrx proxy = BuddyRelationLoaderPrxHelper.checkedCast(prx);
      proxy.reload(host);
      proxy.reload(guest);

      // BuddyRelationTripodCacheAdapter.getInstance().load(host);
      // BuddyRelationTripodCacheAdapter.getInstance().load(guest);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  static void state(int host, int guest) {
//    BuddyRelation br = BuddyRelationTripodCacheAdapter.getInstance().getRelation(host, guest);
//    System.out.println("BuddyRelationTripodCacheAdapter: " + br);
    for (int i = 0; i < 4; ++i) {
      try {
        String endpoint = "M@BuddyRelationCache" + i;
        Ice.ObjectPrx prx = Channel.newBuddyChannel("").getCommunicator().stringToProxy(endpoint);
        BuddyRelationCacheManagerPrx proxy = BuddyRelationCacheManagerPrxHelper.checkedCast(prx);
        System.out.println("endpoint:" + endpoint + "\tisValid():" + proxy.isValid());

        int fromId = host;
        int toId = guest;
        Relationship relation = new Relationship();
        relation.from = fromId;
        relation.to = guest;
        xce.buddy.BuddyDesc desc = xce.buddy.BuddyDesc.NoPath;
        desc = proxy.getRelation(relation);
        System.out.println(proxy + "\t" + desc);

      } catch (Exception e) {
        e.printStackTrace();
      }
    }

  }

  public static void main(String[] args) {
    state(236988432, 229804496);
    System.exit(0);
  }

}

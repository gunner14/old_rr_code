package com.xiaonei.xce.footprintunite;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import xce.footprint.HomeFootprintSet.HomeFootprintSetData;
import xce.footprint.HomeFootprintSet.HomeStepData;
import xce.footprint.HomeStep;
import xce.footprint.UserViewCount.UserViewCountData;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.tripod.client.TripodClient;

public class FootprintTripodClient {
  final private int SHORT_FOOTPRINT_SIZE = 9;
  final private int LONG_FOOTPRINT_SIZE = 39;
  public static int ALL_FOOTPRINT_SIZE = 48;
  final private String ZK_ADDRESS =
    "UserZooKeeper1.n.xiaonei.com:2181,UserZooKeeper2.n.xiaonei.com:2181,UserZooKeeper3.n.xiaonei.com"
    + ":2181,UserZooKeeper4.n.xiaonei.com:2181,UserZooKeeper5.n.xiaonei.com:2181/Tripod";

  final private String ZK_NAMESPACE = "ne1";

  private TripodClient shortFootprintClient = new TripodClient(ZK_ADDRESS, ZK_NAMESPACE, "FootprintHead");
  private TripodClient longFootprintClient = new TripodClient(ZK_ADDRESS, ZK_NAMESPACE, "FootprintBody");
  private TripodClient userViewCountClient = new TripodClient(ZK_ADDRESS, ZK_NAMESPACE, "UserViewCount");
  private class FootprintSet {
    private List<HomeStep> steps_ = new ArrayList<HomeStep>();
    private int owner_ = 0;
    
    public void mergeFootprintSet(final FootprintSet footprintSet) {
      if (owner_ == footprintSet.getOwner()) {
        addHomeSteps(footprintSet.getHomeSteps());
      }
    }
    
    public List<HomeStep> getHomeSteps() {
      List<HomeStep> copySteps = new ArrayList<HomeStep>();
      copySteps.addAll(steps_);
      return copySteps;
    }
    
    public void addHomeSteps(final List<HomeStep> steps) {
      for (HomeStep step : steps) {
        steps_.add(new HomeStep(step.visitor, step.timestamp, step.gid));
      }
    }
    
    public int getOwner() {
      return owner_;
    }
    
    public FootprintSet(int owner) {
      owner_ = owner;
    }
    
    public FootprintSet(final byte[] byteArray) {
      try {
        HomeFootprintSetData data = HomeFootprintSetData.parseFrom(byteArray);
        owner_ = (int)data.getOwnerId();
        for (HomeStepData step : data.getFootprintsList()) {
          steps_.add(new HomeStep(step.getVisitor(), step.getTimestamp(), step.getGid()));
        }
      } catch (InvalidProtocolBufferException e) {
         e.printStackTrace();
      }
    }
    
    HomeFootprintResult parseToHomeFootprint(int begin, int nlimit, int tlimit) {
      int userViewCount = getUserViewCount(owner_);
      List<HomeStep> steps = new ArrayList<HomeStep>();
      for (int i = begin; i < steps_.size(); ++i) {
        HomeStep tempStep = steps_.get(i);
        if (steps.size() >= nlimit || (tlimit > 0 && tempStep.timestamp < tlimit))
          break;
        steps.add(new HomeStep(tempStep.visitor, tempStep.timestamp, tempStep.gid));
      }
      HomeStep[] stepArr = new HomeStep[steps.size()];
      steps.toArray(stepArr);
      HomeFootprintResult result = new HomeFootprintResult(owner_, stepArr, userViewCount);
      return result;
    }
  }
  private FootprintTripodClient() {
  }
  
  private static FootprintTripodClient instance_ = new FootprintTripodClient();
  
  public static FootprintTripodClient getInstance() {
    return instance_;
  }
  
  public HomeFootprintResult getAll(int owner, int begin, int nLimit, int tLimit) {
    FootprintSet footprintSet = null;
    if (begin > SHORT_FOOTPRINT_SIZE) {
      footprintSet = get(owner, 1);
      return footprintSet.parseToHomeFootprint(begin - SHORT_FOOTPRINT_SIZE, nLimit, tLimit);
    }
    if (begin + nLimit > SHORT_FOOTPRINT_SIZE) {
      footprintSet = get(owner, 2);
      return footprintSet.parseToHomeFootprint(begin, nLimit, tLimit);
    }
    footprintSet = get(owner, 0);
    return footprintSet.parseToHomeFootprint(begin, nLimit, tLimit);
  }
  
  /***
   * 
   * @param owner
   * @param needPart, 0表示只需要前9个HomeStep, 1表示需要9-47这39个HomeStep, 2表示需要全部48个HomeStep
   * @return
   */
  private FootprintSet get(int owner, int needPart) {
    FootprintSet footprintSet = null;
    if (needPart == 0) {
      footprintSet = getFootprintSetFromTriopd(shortFootprintClient, owner);
    } else if (needPart == 1) {
      footprintSet = getFootprintSetFromTriopd(longFootprintClient, owner);
    } else {
      footprintSet = getFootprintSetFromTriopd(shortFootprintClient, owner);
      FootprintSet tempFootprintSet = getFootprintSetFromTriopd(longFootprintClient, owner);
      footprintSet.mergeFootprintSet(tempFootprintSet);
    }
    return footprintSet;
  }
  
  public int getUserViewCount(int owner) {
    List<String> keys = new ArrayList<String>();
    keys.add(String.valueOf(owner));
    List<String> missedKeys = new ArrayList<String>();
    Map<String, byte[]> dataMap = userViewCountClient.get(keys, missedKeys);
    if (!missedKeys.isEmpty()) {
      userViewCountClient.load(missedKeys, 0, false);
    }
    if (dataMap.containsKey(String.valueOf(owner))) {
      try {
        UserViewCountData data = UserViewCountData.parseFrom(dataMap.get(String.valueOf(owner)));
        return data.getViewCount();
      } catch (InvalidProtocolBufferException e) {
        e.printStackTrace();
      }
    } else {
      return 0;
    }
    return 0;
  }
  
  private FootprintSet getFootprintSetFromTriopd(final TripodClient client, int owner) {
    List<String> keys = new ArrayList<String>();
    keys.add(String.valueOf(owner));
    List<String> missedKeys = new ArrayList<String>();
    Map<String, byte[]> dataMap = client.get(keys, missedKeys);
    client.load(missedKeys, 0, false);
    client.load(keys, 0, false);
    if (dataMap.containsKey(String.valueOf(owner))) {
      return new FootprintSet(dataMap.get(String.valueOf(owner)));
    } else {
      return new FootprintSet(owner);
    }
  }
}

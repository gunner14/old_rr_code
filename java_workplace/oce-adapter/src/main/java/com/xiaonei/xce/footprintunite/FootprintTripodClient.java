package com.xiaonei.xce.footprintunite;

import java.util.ArrayList;
import java.util.List;

import xce.footprint.HomeFootprintSet.HomeFootprintSetData;
import xce.footprint.HomeFootprintSet.HomeStepData;
import xce.footprint.HomeStep;
import xce.footprint.UserViewCount.UserViewCountData;
import xce.tripod.TripodClient;
import xce.tripod.TripodException;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.log.Oce;

public class FootprintTripodClient {

    /** 单例 */
    private static FootprintTripodClient instance = null;

    private static final String NAMESPACE_ID = "xce_foot_print";

    private static final String HEAD_BIZ_ID = "foot_print_head";

    private static final String BODY_BIZ_ID = "foot_print_body";

    private static final String VIEW_COUNT_BIZ_ID = "user_view_count";

    private static final int SHORT_FOOTPRINT_SIZE = 9;

    private static final int LONG_FOOTPRINT_SIZE = 39;

    public static int ALL_FOOTPRINT_SIZE = 48;

    private TripodClient headTripodClient = null;

    private TripodClient bodyTripodClient = null;

    private TripodClient viewCountTripodClient = null;

    private FootprintTripodClient() {
        headTripodClient = new TripodClient(NAMESPACE_ID, HEAD_BIZ_ID);
        bodyTripodClient = new TripodClient(NAMESPACE_ID, BODY_BIZ_ID);
        viewCountTripodClient = new TripodClient(NAMESPACE_ID, VIEW_COUNT_BIZ_ID);
    }

    public static FootprintTripodClient getInstance() {
        if (instance == null) {
            synchronized (FootprintTripodClient.class) {
                if (instance == null) {
                    instance = new FootprintTripodClient();
                }
            }
        }
        return instance;
    }

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
                owner_ = (int) data.getOwnerId();
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
                if (steps.size() >= nlimit || (tlimit > 0 && tempStep.timestamp < tlimit)) break;
                steps.add(new HomeStep(tempStep.visitor, tempStep.timestamp, tempStep.gid));
            }
            HomeStep[] stepArr = new HomeStep[steps.size()];
            steps.toArray(stepArr);
            HomeFootprintResult result = new HomeFootprintResult(owner_, stepArr, userViewCount);
            return result;
        }
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
     * @param needPart , 0表示只需要前9个HomeStep, 1表示需要9-47这39个HomeStep,
     *        2表示需要全部48个HomeStep
     * @return
     */
    private FootprintSet get(int owner, int needPart) {
        FootprintSet footprintSet = null;
        if (needPart == 0) {
            footprintSet = getFootprintSetFromTriopd(headTripodClient, owner);
        } else if (needPart == 1) {
            footprintSet = getFootprintSetFromTriopd(bodyTripodClient, owner);
        } else {
            footprintSet = getFootprintSetFromTriopd(headTripodClient, owner);
            FootprintSet tempFootprintSet = getFootprintSetFromTriopd(bodyTripodClient, owner);
            footprintSet.mergeFootprintSet(tempFootprintSet);
        }
        return footprintSet;
    }

    public int getUserViewCount(int owner) {
        byte[] byteData = null;
        try {
            byteData = viewCountTripodClient.get(String.valueOf(owner), 0);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUserViewCount ownerId=" + owner
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            UserViewCountData data = UserViewCountData.parseFrom(byteData);
            return data.getViewCount();
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return 0;
    }

    private FootprintSet getFootprintSetFromTriopd(final TripodClient client, int owner) {
        byte[] byteData = null;
        try {
            byteData = client.get(String.valueOf(owner), 0);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getFootprintSetFromTriopd ownerId=" + owner
                            + " TripodException msg:" + e.getMessage());
        }
        return new FootprintSet(byteData);
    }
}

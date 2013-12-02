package mop.hi.oce.adapter.impl;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.Network;
import mop.hi.oce.domain.network.NetworkBigData;
import mop.hi.oce.domain.network.NetworkBigObject;
import mop.hi.oce.domain.network.NetworkBigObjectN;
import mop.hi.oce.domain.network.RegionInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;
import xce.tripod.MultiBizTripodClient;
import xce.tripod.TripodClient;
import xce.tripod.TripodException;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.tripod.proto.UserNetworkProto.PbCollegeInfo;
import com.renren.tripod.proto.UserNetworkProto.PbCollegeInfo.PbCollege;
import com.renren.tripod.proto.UserNetworkProto.PbElementarySchoolInfo;
import com.renren.tripod.proto.UserNetworkProto.PbElementarySchoolInfo.PbElementarySchool;
import com.renren.tripod.proto.UserNetworkProto.PbHighSchoolInfo;
import com.renren.tripod.proto.UserNetworkProto.PbHighSchoolInfo.PbHighSchool;
import com.renren.tripod.proto.UserNetworkProto.PbJuniorHighSchoolInfo;
import com.renren.tripod.proto.UserNetworkProto.PbJuniorHighSchoolInfo.PbJuniorHighSchool;
import com.renren.tripod.proto.UserNetworkProto.PbNetworkHistory;
import com.renren.tripod.proto.UserNetworkProto.PbNetworkHistory.PbNetwork;
import com.renren.tripod.proto.UserNetworkProto.PbRegionInfo;
import com.renren.tripod.proto.UserNetworkProto.PbRegionInfo.PbRegion;
import com.renren.tripod.proto.UserNetworkProto.PbUniversityInfo;
import com.renren.tripod.proto.UserNetworkProto.PbUniversityInfo.PbUniversity;
import com.renren.tripod.proto.UserNetworkProto.PbWorkplaceInfo;
import com.renren.tripod.proto.UserNetworkProto.PbWorkplaceInfo.PbWorkplace;
import com.xiaonei.xce.log.Oce;
import com.xiaonei.xce.log.TimeCost;

public class NetworkTripodAdapter {

    /** 单例 */
    private static NetworkTripodAdapter instance = null;

    private static final String NAMESPACE_ID = "xce_network";

    private static final String COLLEGE_BIZ_ID = "user_network_college";

    private static final String ELEMENTARY_SCHOOL_BIZ_ID = "user_network_elementary_school";

    private static final String HIGH_SCHOOL_BIZ_ID = "user_network_high_school";

    private static final String JUNIOR_HIGH_SCHOOL_BIZ_ID = "user_network_junior_high_school";

    private static final String NETWORK_BIZ_ID = "user_network_network";

    private static final String REGION_BIZ_ID = "user_network_region";

    private static final String UNIVERSITY_BIZ_ID = "user_network_university";

    private static final String WORKPLACE_BIZ_ID = "user_network_workplace";

    private static final int EXPIRE_TIME = 30 * 24 * 3600;//一个月

    private TripodClient collegeClient;

    private TripodClient elementarySchoolClient;

    private TripodClient highSchoolClient;

    private TripodClient juniorHighSchoolClient;

    private TripodClient networkClient;

    private TripodClient regionClient;

    private TripodClient universityClient;

    private TripodClient workplaceClient;

    private MultiBizTripodClient multiBizClient;

    private NetworkTripodAdapter() {
        collegeClient = new TripodClient(NAMESPACE_ID, COLLEGE_BIZ_ID);
        elementarySchoolClient = new TripodClient(NAMESPACE_ID, ELEMENTARY_SCHOOL_BIZ_ID);
        highSchoolClient = new TripodClient(NAMESPACE_ID, HIGH_SCHOOL_BIZ_ID);
        juniorHighSchoolClient = new TripodClient(NAMESPACE_ID, JUNIOR_HIGH_SCHOOL_BIZ_ID);
        networkClient = new TripodClient(NAMESPACE_ID, NETWORK_BIZ_ID);
        regionClient = new TripodClient(NAMESPACE_ID, REGION_BIZ_ID);
        universityClient = new TripodClient(NAMESPACE_ID, UNIVERSITY_BIZ_ID);
        workplaceClient = new TripodClient(NAMESPACE_ID, WORKPLACE_BIZ_ID);
        multiBizClient = new MultiBizTripodClient(NAMESPACE_ID);
    }

    public static NetworkTripodAdapter getInstance() {
        if (instance == null) {
            synchronized (NetworkTripodAdapter.class) {
                if (instance == null) {
                    instance = new NetworkTripodAdapter();
                }
            }
        }
        return instance;
    }

    // **************** network********************************
    public int getNetworkCountByStage(int userId, int stage) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetworkCountByStage userId: "
                + userId + " stage: " + stage);
        int result = 0;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkCountByStage userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infosPb = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infosPb) {
                        if (infoPb.getStage() == stage || stage == -1) {
                            ++result;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkCountByStage userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkCountByStage userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public int getValidNetworkCountByStage(int userId, int stage) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getValidNetworkCountByStage userId: "
                + userId + " stage: " + stage);
        int result = 0;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getValidNetworkCountByStage userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb;
                dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infosPb = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infosPb) {
                        if (infoPb.getStage() == stage || stage == -1) {
                            if (infoPb.getStatus() == 0 || infoPb.getStatus() == -1) {
                                ++result;
                            }
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getValidNetworkCountByStage userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getValidNetworkCountByStage userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<Network> getNetworkList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetworkList userId=" + userId);
        List<Network> result = new ArrayList<Network>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infoPbs) {
                        Network info = new Network();
                        info.set(userId, infoPb);
                        result.add(info);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkList userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkList userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<Network> getNetworkListByStage(int userId, int stage) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetworkListByStage userId: "
                + userId + " stage: " + stage);
        List<Network> result = new ArrayList<Network>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStage userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infoPbs) {
                        if (infoPb.getStage() == stage || stage == -1) {
                            Network info = new Network();
                            info.set(userId, infoPb);
                            result.add(info);
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getNetworkListByStage userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getNetworkListByStage userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<Network> getNetworkListByStatus(int userId, int status) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetworkListByStatus userId: "
                + userId + " status: " + status);
        List<Network> result = new ArrayList<Network>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infoPbs) {
                        if (infoPb.getStatus() == status) {
                            Network info = new Network();
                            info.set(userId, infoPb);
                            result.add(info);
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStatus userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStatus userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<Network> getNetworkListByStageAndStatus(int userId, int stage, int[] status) {
        TimeCost cost = TimeCost
                .begin("NetworkTripodAdapter.getNetworkListByStageAndStatus userId: " + userId
                        + " stage: " + stage);
        List<Network> result = new ArrayList<Network>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStageAndStatus userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    List<Integer> statusList = new ArrayList<Integer>();
                    for (int i : status) {
                        statusList.add(i);
                    }
                    for (PbNetwork infoPb : infoPbs) {
                        if (infoPb.getStage() == stage || stage == -1) {
                            if (statusList.contains(infoPb.getStatus())) {
                                Network info = new Network();
                                info.set(userId, infoPb);
                                result.add(info);
                            }
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStageAndStatus userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStageAndStatus userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<Network> getNetworkListByStatus(int userId, int[] status) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetworkListByStatus userId: "
                + userId);
        List<Network> result = new ArrayList<Network>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStatus userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    List<Integer> statusList = new ArrayList<Integer>();
                    for (int i : status) {
                        statusList.add(i);
                    }
                    for (PbNetwork infoPb : infoPbs) {
                        if (statusList.contains(infoPb.getStatus())) {
                            Network info = new Network();
                            info.set(userId, infoPb);
                            result.add(info);
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStatus userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkListByStatus userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public Network getNetwork(int userId, int networkId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetwork userId: " + userId
                + " networkId: " + networkId);
        Network result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetwork userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infoPbs) {
                        if (infoPb.getNetworkId() == networkId) {
                            result = new Network();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetwork userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetwork userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public Network getRegionalNetwork(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getRegionalNetwork userId: " + userId);
        Network result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionalNetwork userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infoPbs) {
                        if (infoPb.getNetworkId() >= 60000000 && infoPb.getNetworkId() <= 70000000
                                && (infoPb.getStatus() == 0 || infoPb.getStatus() == -1)) {
                            result = new Network();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionalNetwork userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionalNetwork userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public boolean isExistInNetwork(int userId, int networkId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.isExistInNetwork userId: " + userId
                + " networkId: " + networkId);
        boolean result = false;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = networkClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".isExistInNetwork userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbNetwork> infoPbs = dataPb.getNetworksList();
                    for (PbNetwork infoPb : infoPbs) {
                        if (infoPb.getNetworkId() == networkId && infoPb.getStatus() <= 0) {
                            result = true;
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".isExistInNetwork userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".isExistInNetwork userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadNetworkList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.reloadNetworkList userid: " + userId);
        try {
            networkClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadNetworkList userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }
    }

    // ******************************highSchool*****************************************
    public int getHighSchoolInfoCount(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getHighSchoolInfoCount userId: "
                + userId);
        int result = 0;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = highSchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getHighSchoolInfoCount userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbHighSchoolInfo dataPb = PbHighSchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    result = dataPb.getHighschoolsCount();
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getHighSchoolInfoCount userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getHighSchoolInfoCount userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<HighSchoolInfo> getHighSchoolInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getHighSchoolInfoList userId: "
                + userId);
        List<HighSchoolInfo> result = new ArrayList<HighSchoolInfo>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = highSchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getHighSchoolInfoList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbHighSchoolInfo dataPb = PbHighSchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbHighSchool> infosPb = dataPb.getHighschoolsList();
                    for (PbHighSchool infoPb : infosPb) {
                        HighSchoolInfo info = new HighSchoolInfo();
                        info.set(userId, infoPb);
                        result.add(info);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getHighSchoolInfoList userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getHighSchoolInfoList userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public HighSchoolInfo getHighSchoolInfoBySchoolId(int userId, int schoolId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getHighSchoolInfoBySchoolId userId: "
                + userId + " schoolid: " + schoolId);
        HighSchoolInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = highSchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getHighSchoolInfoBySchoolId userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbHighSchoolInfo dataPb = PbHighSchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbHighSchool> infosPb = dataPb.getHighschoolsList();
                    for (PbHighSchool infoPb : infosPb) {
                        if (infoPb.getHighSchoolId() == schoolId) {
                            result = new HighSchoolInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getHighSchoolInfoBySchoolId userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getHighSchoolInfoBySchoolId userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadHighSchoolInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.reloadHighSchoolInfoList userid: "
                + userId);
        try {
            highSchoolClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadHighSchoolInfoList userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }

    }

    // ********************************University***************************************
    public int getUniversityInfoCount(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getUniversityInfoCount userId: "
                + userId);
        int result = 0;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = universityClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUniversityInfoCount userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbUniversityInfo dataPb = PbUniversityInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    result = dataPb.getUniversitiesCount();
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUniversityInfoCount userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUniversityInfoCount userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<UniversityInfo> getUniversityInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getUniversityInfoList userId: "
                + userId);
        List<UniversityInfo> result = new ArrayList<UniversityInfo>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = universityClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUniversityInfoList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbUniversityInfo dataPb = PbUniversityInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbUniversity> infosPb = dataPb.getUniversitiesList();
                    for (PbUniversity infoPb : infosPb) {
                        UniversityInfo info = new UniversityInfo();
                        info.set(userId, infoPb);
                        result.add(info);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getUniversityInfoList userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getUniversityInfoList userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public UniversityInfo getUniversityInfoByUnivId(int userId, int univId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getUniversityInfoByUnivId userId: "
                + userId + " univid: " + univId);
        UniversityInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = universityClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUniversityInfoByUnivId userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbUniversityInfo dataPb = PbUniversityInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbUniversity> infosPb = dataPb.getUniversitiesList();
                    for (PbUniversity infoPb : infosPb) {
                        if (infoPb.getUniversityId() == univId) {
                            result = new UniversityInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUniversityInfoByUnivId userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getUniversityInfoByUnivId userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadUniversityInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.reloadUniversityInfoList userid: "
                + userId);
        try {
            universityClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadUniversityInfoList userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }
    }

    // **********************Workplace***********************************
    public int getWorkplaceInfoCount(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getWorkplaceInfoCount userId: "
                + userId);
        int result = 0;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = workplaceClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getWorkplaceInfoCount userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbWorkplaceInfo dataPb = PbWorkplaceInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    result = dataPb.getWorkplacesCount();
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getWorkplaceInfoCount userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger()
                    .error(this.getClass().getName() + ".getWorkplaceInfoCount userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<WorkplaceInfo> getWorkplaceInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getWorkplaceInfoList userId: "
                + userId);
        List<WorkplaceInfo> result = new ArrayList<WorkplaceInfo>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = workplaceClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getWorkplaceInfoList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbWorkplaceInfo dataPb = PbWorkplaceInfo.parseFrom(dataRaw);
                List<PbWorkplace> infosPb = dataPb.getWorkplacesList();
                for (PbWorkplace infoPb : infosPb) {
                    WorkplaceInfo info = new WorkplaceInfo();
                    info.set(userId, infoPb);
                    result.add(info);
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getWorkplaceInfoList userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getWorkplaceInfoList userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public WorkplaceInfo getWorkplaceInfoByWorkId(int userId, int workId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getWorkplaceInfoByWorkId userId: "
                + userId + " workId: " + workId);
        WorkplaceInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = workplaceClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getWorkplaceInfoByWorkId userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbWorkplaceInfo dataPb = PbWorkplaceInfo.parseFrom(dataRaw);
                List<PbWorkplace> infosPb = dataPb.getWorkplacesList();
                for (PbWorkplace infoPb : infosPb) {
                    if (infoPb.getWorkplaceId() == workId) {
                        result = new WorkplaceInfo();
                        result.set(userId, infoPb);
                        break;
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getWorkplaceInfoByWorkId userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getWorkplaceInfoByWorkId userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadWorkplaceInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.reloadWorkplaceInfoList userid: "
                + userId);
        try {
            workplaceClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadWorkplaceInfoList userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }
    }

    // ********************************College***************************************

    public int getCollegeInfoCount(int userId) {
        TimeCost cost = TimeCost
                .begin("NetworkTripodAdapter.getCollegeInfoCount userId: " + userId);
        int result = 0;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = collegeClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoCount userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbCollegeInfo dataPb = PbCollegeInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    result = dataPb.getCollegesCount();
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoCount userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoCount userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public List<CollegeInfo> getCollegeInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getCollegeInfoList userId: " + userId);
        List<CollegeInfo> result = new ArrayList<CollegeInfo>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = collegeClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbCollegeInfo dataPb = PbCollegeInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbCollege> infosPb = dataPb.getCollegesList();
                    for (PbCollege infoPb : infosPb) {
                        CollegeInfo info = new CollegeInfo();
                        info.set(userId, infoPb);
                        result.add(info);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoList userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoList userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public CollegeInfo getCollegeInfo(int userId, int id) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getCollegeInfo userId: " + userId
                + " id: " + id);
        CollegeInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = collegeClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfo userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbCollegeInfo dataPb = PbCollegeInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbCollege> infosPb = dataPb.getCollegesList();
                    for (PbCollege infoPb : infosPb) {
                        if (infoPb.getId() == id) {
                            result = new CollegeInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfo userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfo userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public CollegeInfo getCollegeInfoByCollegeId(int userId, int collegeId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getCollegeInfoByCollegeId userId: "
                + userId + " collegeid: " + collegeId);
        CollegeInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = collegeClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoByCollegeId userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbCollegeInfo dataPb = PbCollegeInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbCollege> infosPb = dataPb.getCollegesList();
                    for (PbCollege infoPb : infosPb) {
                        if (infoPb.getCollegeId() == collegeId) {
                            result = new CollegeInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoByCollegeId userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getCollegeInfoByCollegeId userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadCollegeInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.reloadCollegeInfoList userid: "
                + userId);
        try {
            collegeClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadCollegeInfoList userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }
    }

    // *********************************ElementarySchool**************************************
    public List<ElementarySchoolInfo> getElementarySchoolInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getElementarySchoolInfoList userId: "
                + userId);
        List<ElementarySchoolInfo> result = new ArrayList<ElementarySchoolInfo>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = elementarySchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfoList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbElementarySchoolInfo dataPb = PbElementarySchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbElementarySchool> infosPb = dataPb.getElementaryschoolsList();
                    for (PbElementarySchool infoPb : infosPb) {
                        ElementarySchoolInfo info = new ElementarySchoolInfo();
                        info.set(userId, infoPb);
                        result.add(info);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfoList userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfoList userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public ElementarySchoolInfo getElementarySchoolInfo(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getElementarySchoolInfo userId: "
                + userId);
        ElementarySchoolInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = elementarySchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfo userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbElementarySchoolInfo dataPb = PbElementarySchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    PbElementarySchool infoPb = dataPb.getElementaryschools(0);
                    if (infoPb != null) {
                        result = new ElementarySchoolInfo();
                        result.set(userId, infoPb);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfo userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfo userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public ElementarySchoolInfo getElementarySchoolInfo(int userId, int id) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getElementarySchoolInfo userId: "
                + userId + " id: " + id);
        ElementarySchoolInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = elementarySchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfo userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbElementarySchoolInfo dataPb = PbElementarySchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbElementarySchool> infosPb = dataPb.getElementaryschoolsList();
                    for (PbElementarySchool infoPb : infosPb) {
                        if (infoPb.getId() == id) {
                            result = new ElementarySchoolInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfo userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfo userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public ElementarySchoolInfo getElementarySchoolInfoBySchoolId(int userId, int schoolId) {
        TimeCost cost = TimeCost
                .begin("NetworkTripodAdapter.getElementarySchoolInfoBySchoolId userId: " + userId
                        + " schoolid: " + schoolId);
        ElementarySchoolInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = elementarySchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfoBySchoolId userId="
                            + userId + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbElementarySchoolInfo dataPb = PbElementarySchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbElementarySchool> infosPb = dataPb.getElementaryschoolsList();
                    for (PbElementarySchool infoPb : infosPb) {
                        if (infoPb.getElementarySchoolId() == schoolId) {
                            result = new ElementarySchoolInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfoBySchoolId userId="
                            + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getElementarySchoolInfoBySchoolId userId="
                            + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadElementarySchoolInfo(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.reloadElementarySchoolInfo userid: "
                + userId);
        try {
            elementarySchoolClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadElementarySchoolInfo userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }
    }

    // *********************************JuniorHighSchool**************************************

    public List<JuniorHighSchoolInfo> getJuniorHighSchoolInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getJuniorHighSchoolInfoList userId: "
                + userId);
        List<JuniorHighSchoolInfo> result = new ArrayList<JuniorHighSchoolInfo>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = juniorHighSchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfoList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbJuniorHighSchoolInfo dataPb = PbJuniorHighSchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbJuniorHighSchool> infosPb = dataPb.getJuniorhighschoolsList();
                    for (PbJuniorHighSchool infoPb : infosPb) {
                        JuniorHighSchoolInfo info = new JuniorHighSchoolInfo();
                        info.set(userId, infoPb);
                        result.add(info);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfoList userId=" + userId
                            + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfoList userId=" + userId
                            + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public JuniorHighSchoolInfo getJuniorHighSchoolInfo(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getJuniorHighSchoolInfo userId: "
                + userId);
        JuniorHighSchoolInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = juniorHighSchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbJuniorHighSchoolInfo dataPb = PbJuniorHighSchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    PbJuniorHighSchool infoPb = dataPb.getJuniorhighschools(0);
                    if (infoPb != null) {
                        result = new JuniorHighSchoolInfo();
                        result.set(userId, infoPb);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public JuniorHighSchoolInfo getJuniorHighSchoolInfo(int userId, int id) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getJuniorHighSchoolInfo userId: "
                + userId + " id: " + id);
        JuniorHighSchoolInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = juniorHighSchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbJuniorHighSchoolInfo dataPb = PbJuniorHighSchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbJuniorHighSchool> infosPb = dataPb.getJuniorhighschoolsList();
                    for (PbJuniorHighSchool infoPb : infosPb) {
                        if (infoPb.getId() == id) {
                            result = new JuniorHighSchoolInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public JuniorHighSchoolInfo getJuniorHighSchoolInfoBySchoolId(int userId, int schoolId) {
        TimeCost cost = TimeCost
                .begin("NetworkTripodAdapter.getJuniorHighSchoolInfoBySchoolId userId: " + userId
                        + " schoolid: " + schoolId);
        JuniorHighSchoolInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = juniorHighSchoolClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfoBySchoolId userId="
                            + userId + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbJuniorHighSchoolInfo dataPb = PbJuniorHighSchoolInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbJuniorHighSchool> infosPb = dataPb.getJuniorhighschoolsList();
                    for (PbJuniorHighSchool infoPb : infosPb) {
                        if (infoPb.getJuniorHighSchoolId() == schoolId) {
                            result = new JuniorHighSchoolInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId + " "
                            + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getJuniorHighSchoolInfo userId=" + userId + " "
                            + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadJuniorHighSchoolInfo(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.reloadJuniorHighSchoolInfo userid: "
                + userId);
        try {
            juniorHighSchoolClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadJuniorHighSchoolInfo userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }
    }

    // *********************************Region**************************************

    public List<RegionInfo> getRegionInfoList(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getRegionInfoList userId: " + userId);
        List<RegionInfo> result = new ArrayList<RegionInfo>();
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = regionClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionInfoList userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbRegionInfo dataPb = PbRegionInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbRegion> infosPb = dataPb.getRegionsList();
                    for (PbRegion infoPb : infosPb) {
                        RegionInfo info = new RegionInfo();
                        info.set(userId, infoPb);
                        result.add(info);
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionInfoList userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionInfoList userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public RegionInfo getRegionInfo(int userId, int regionId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapterMCImpl.getRegionInfo userId: " + userId
                + " regionid: " + regionId);
        RegionInfo result = null;
        if (userId <= 0) {
            return result;
        }
        byte[] dataRaw = null;
        try {
            dataRaw = regionClient.get(String.valueOf(userId), EXPIRE_TIME);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionInfo userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            if (dataRaw != null && dataRaw.length != 0) {
                PbRegionInfo dataPb = PbRegionInfo.parseFrom(dataRaw);
                if (dataPb.getUserid() == userId) {
                    List<PbRegion> infosPb = dataPb.getRegionsList();
                    for (PbRegion infoPb : infosPb) {
                        if (infoPb.getRegionId() == regionId) {
                            result = new RegionInfo();
                            result.set(userId, infoPb);
                            break;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionInfoList userId=" + userId + " " + e);
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getRegionInfoList userId=" + userId + " " + e);
            throw e;
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public void reloadRegionInfo(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getHighSchoolInfoBySchoolId userid: "
                + userId);
        try {
            regionClient.load(String.valueOf(userId), EXPIRE_TIME, false);
        } catch (TripodException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".reloadRegionInfo userId=" + userId
                            + " TripodException msg:" + e.getMessage());
        } finally {
            cost.endFinally();
        }
    }

    // *********************************NetworkBig**************************************

    private List<Network> getNetworkListfromMultiBizData(int userId, Map<String, byte[]> dataRawMap)
            throws InvalidProtocolBufferException {
        List<Network> result = new ArrayList<Network>();
        PbNetworkHistory dataPb = PbNetworkHistory.parseFrom(dataRawMap.get(NETWORK_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbNetwork> infoPbs = dataPb.getNetworksList();
            for (PbNetwork infoPb : infoPbs) {
                Network info = new Network();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    private List<HighSchoolInfo> getHighSchoolInfoListfromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        List<HighSchoolInfo> result = new ArrayList<HighSchoolInfo>();
        PbHighSchoolInfo dataPb = PbHighSchoolInfo.parseFrom(dataRawMap.get(HIGH_SCHOOL_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbHighSchool> infosPb = dataPb.getHighschoolsList();
            for (PbHighSchool infoPb : infosPb) {
                HighSchoolInfo info = new HighSchoolInfo();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    private List<UniversityInfo> getUniversityInfoListfromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        List<UniversityInfo> result = new ArrayList<UniversityInfo>();
        PbUniversityInfo dataPb = PbUniversityInfo.parseFrom(dataRawMap.get(UNIVERSITY_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbUniversity> infosPb = dataPb.getUniversitiesList();
            for (PbUniversity infoPb : infosPb) {
                UniversityInfo info = new UniversityInfo();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    private List<WorkplaceInfo> getWorkplaceInfoListfromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        List<WorkplaceInfo> result = new ArrayList<WorkplaceInfo>();
        PbWorkplaceInfo dataPb = PbWorkplaceInfo.parseFrom(dataRawMap.get(WORKPLACE_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbWorkplace> infosPb = dataPb.getWorkplacesList();
            for (PbWorkplace infoPb : infosPb) {
                WorkplaceInfo info = new WorkplaceInfo();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    private List<CollegeInfo> getCollegeInfoListfromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        List<CollegeInfo> result = new ArrayList<CollegeInfo>();
        PbCollegeInfo dataPb = PbCollegeInfo.parseFrom(dataRawMap.get(COLLEGE_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbCollege> infosPb = dataPb.getCollegesList();
            for (PbCollege infoPb : infosPb) {
                CollegeInfo info = new CollegeInfo();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    private List<ElementarySchoolInfo> getElementarySchoolInfoListfromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        List<ElementarySchoolInfo> result = new ArrayList<ElementarySchoolInfo>();
        PbElementarySchoolInfo dataPb = PbElementarySchoolInfo.parseFrom(dataRawMap
                .get(ELEMENTARY_SCHOOL_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbElementarySchool> infosPb = dataPb.getElementaryschoolsList();
            for (PbElementarySchool infoPb : infosPb) {
                ElementarySchoolInfo info = new ElementarySchoolInfo();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    private ElementarySchoolInfo getElementarySchoolInfofromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        ElementarySchoolInfo result = null;
        PbElementarySchoolInfo dataPb = PbElementarySchoolInfo.parseFrom(dataRawMap
                .get(ELEMENTARY_SCHOOL_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            PbElementarySchool infoPb = dataPb.getElementaryschools(0);
            if (infoPb != null) {
                result = new ElementarySchoolInfo();
                result.set(userId, infoPb);
            }
        }
        return result;
    }

    private List<JuniorHighSchoolInfo> getJuniorHighSchoolInfoListfromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        List<JuniorHighSchoolInfo> result = new ArrayList<JuniorHighSchoolInfo>();
        PbJuniorHighSchoolInfo dataPb = PbJuniorHighSchoolInfo.parseFrom(dataRawMap
                .get(JUNIOR_HIGH_SCHOOL_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbJuniorHighSchool> infosPb = dataPb.getJuniorhighschoolsList();
            for (PbJuniorHighSchool infoPb : infosPb) {
                JuniorHighSchoolInfo info = new JuniorHighSchoolInfo();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    private JuniorHighSchoolInfo getJuniorHighSchoolInfofromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        JuniorHighSchoolInfo result = null;
        PbJuniorHighSchoolInfo dataPb = PbJuniorHighSchoolInfo.parseFrom(dataRawMap
                .get(JUNIOR_HIGH_SCHOOL_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            PbJuniorHighSchool infoPb = dataPb.getJuniorhighschools(0);
            if (infoPb != null) {
                result = new JuniorHighSchoolInfo();
                result.set(userId, infoPb);
            }
        }
        return result;
    }

    private List<RegionInfo> getRegionInfoListfromMultiBizData(int userId,
            Map<String, byte[]> dataRawMap) throws InvalidProtocolBufferException {
        List<RegionInfo> result = new ArrayList<RegionInfo>();
        PbRegionInfo dataPb = PbRegionInfo.parseFrom(dataRawMap.get(REGION_BIZ_ID));
        if (dataPb.getUserid() == userId) {
            List<PbRegion> infosPb = dataPb.getRegionsList();
            for (PbRegion infoPb : infosPb) {
                RegionInfo info = new RegionInfo();
                info.set(userId, infoPb);
                result.add(info);
            }
        }
        return result;
    }

    @Deprecated
    public mop.hi.oce.domain.network.NetworkBigObject getNetworkBigObject(int userId) {
        TimeCost cost = TimeCost
                .begin("NetworkTripodAdapter.getNetworkBigObject userId: " + userId);
        NetworkBigObject result = null;
        try {
            if (userId > 0) {
                List<String> bizs = new LinkedList<String>();
                bizs.add(UNIVERSITY_BIZ_ID);
                bizs.add(NETWORK_BIZ_ID);
                bizs.add(HIGH_SCHOOL_BIZ_ID);
                bizs.add(WORKPLACE_BIZ_ID);
                Map<String, byte[]> dataRawMap = multiBizClient.multiBizGet(String.valueOf(userId),
                        bizs, EXPIRE_TIME);
                result = new NetworkBigObject();
                result.setHighschools(this
                        .getHighSchoolInfoListfromMultiBizData(userId, dataRawMap));
                result.setNetworks(this.getNetworkListfromMultiBizData(userId, dataRawMap));
                result.setUnivs(this.getUniversityInfoListfromMultiBizData(userId, dataRawMap));
                result.setWorkspaces(this.getWorkplaceInfoListfromMultiBizData(userId, dataRawMap));
            }
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkBigObject userId=" + userId + " " + e);
            throw e;
        } catch (Exception e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkBigObject userId=" + userId + " " + e);
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public NetworkBigObjectN getNetworkBigObjectN(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetworkBigObjectN userId: "
                + userId);
        NetworkBigObjectN result = null;
        try {
            if (userId > 0) {
                List<String> bizs = new LinkedList<String>();
                bizs.add(UNIVERSITY_BIZ_ID);
                bizs.add(NETWORK_BIZ_ID);
                bizs.add(HIGH_SCHOOL_BIZ_ID);
                bizs.add(WORKPLACE_BIZ_ID);
                bizs.add(COLLEGE_BIZ_ID);
                bizs.add(ELEMENTARY_SCHOOL_BIZ_ID);
                bizs.add(JUNIOR_HIGH_SCHOOL_BIZ_ID);
                bizs.add(REGION_BIZ_ID);

                Map<String, byte[]> dataRawMap = multiBizClient.multiBizGet(String.valueOf(userId),
                        bizs, EXPIRE_TIME);

                result = new NetworkBigObjectN();
                result.setHighschools(this
                        .getHighSchoolInfoListfromMultiBizData(userId, dataRawMap));
                result.setNetworks(this.getNetworkListfromMultiBizData(userId, dataRawMap));
                result.setUnivs(this.getUniversityInfoListfromMultiBizData(userId, dataRawMap));
                result.setWorkspaces(this.getWorkplaceInfoListfromMultiBizData(userId, dataRawMap));
                result.setCollages(this.getCollegeInfoListfromMultiBizData(userId, dataRawMap));
                result.setElementaryschool(this.getElementarySchoolInfofromMultiBizData(userId,
                        dataRawMap));
                result.setJuniorhighschool(this.getJuniorHighSchoolInfofromMultiBizData(userId,
                        dataRawMap));
            }
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkBigObjectN userId=" + userId + " " + e);
            throw e;
        } catch (Exception e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkBigObjectN userId=" + userId + " " + e);
        } finally {
            cost.endFinally();
        }
        return result;
    }

    public mop.hi.oce.domain.network.NetworkBigData getNetworkBigData(int userId) {
        TimeCost cost = TimeCost.begin("NetworkTripodAdapter.getNetworkBigData userId: " + userId);
        NetworkBigData result = null;
        try {
            if (userId > 0) {
                List<String> bizs = new LinkedList<String>();
                bizs.add(UNIVERSITY_BIZ_ID);
                bizs.add(NETWORK_BIZ_ID);
                bizs.add(HIGH_SCHOOL_BIZ_ID);
                bizs.add(WORKPLACE_BIZ_ID);
                bizs.add(COLLEGE_BIZ_ID);
                bizs.add(ELEMENTARY_SCHOOL_BIZ_ID);
                bizs.add(JUNIOR_HIGH_SCHOOL_BIZ_ID);
                bizs.add(REGION_BIZ_ID);

                Map<String, byte[]> dataRawMap = multiBizClient.multiBizGet(String.valueOf(userId),
                        bizs, EXPIRE_TIME);

                result = new NetworkBigData();
                result.setHighschools(this
                        .getHighSchoolInfoListfromMultiBizData(userId, dataRawMap));
                result.setNetworks(this.getNetworkListfromMultiBizData(userId, dataRawMap));
                result.setUnivs(this.getUniversityInfoListfromMultiBizData(userId, dataRawMap));
                result.setWorkspaces(this.getWorkplaceInfoListfromMultiBizData(userId, dataRawMap));
                result.setCollages(this.getCollegeInfoListfromMultiBizData(userId, dataRawMap));
                result.setRegions(this.getRegionInfoListfromMultiBizData(userId, dataRawMap));
                result.setElementaryschools(this.getElementarySchoolInfoListfromMultiBizData(
                        userId, dataRawMap));
                result.setJuniorhighschools(this.getJuniorHighSchoolInfoListfromMultiBizData(
                        userId, dataRawMap));
            }
        } catch (Ice.TimeoutException e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkBigData userId=" + userId + " " + e);
            throw e;
        } catch (Exception e) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getNetworkBigData userId=" + userId + " " + e);
        } finally {
            cost.endFinally();
        }
        return result;
    }

    //保留供开发者快速调用
    public static void main(String[] args) {
        System.out.println("--------------------------------");
        System.out.println(NetworkTripodAdapter.getInstance().getNetworkCountByStage(35178, -1));
        System.exit(0);
    }

}

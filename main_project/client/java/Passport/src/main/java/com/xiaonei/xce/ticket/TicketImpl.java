package com.xiaonei.xce.ticket;

import java.util.ArrayList;
import java.util.List;

import xce.tripod.ticket.TicketManagerPrx;
import xce.tripod.ticket.TicketManagerPrxHelper;
import xce.util.tools.MathUtil;
import xce.clusterstate.ReplicatedClusterAdapter;

import com.google.protobuf.InvalidProtocolBufferException;
import xce.tripod.TripodClient;
import xce.tripod.TripodException;

import com.renren.tripod.client.model.TicketProto.TicketData;
import com.xiaonei.xce.log.Oce;
import com.xiaonei.xce.ticket.TicketUTils.TicketUtil;

class TicketImpl implements Ticket {

    private TicketManager _cacheManager;

    private TripodClient _cacheClient;

    public final String NAME_SPACE = "xce_ticket";

    public final String BIZ_ID;

    public TicketImpl(String type) {
        BIZ_ID = type;
        _cacheManager = new TicketManager(type);
        _cacheClient = new TripodClient(NAME_SPACE, BIZ_ID);
    }

    private class TicketManager extends ReplicatedClusterAdapter {

        private final static String ctr_endpoints = "Controller";

        private static final int _interval = 120;

        private TicketManager(String type) {
            super(ctr_endpoints + type, _interval);
        }

        public TicketManagerPrx getProxy(int userId) {
            Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
            if (prx0 != null) {
                return TicketManagerPrxHelper.uncheckedCast(prx0.ice_twoway().ice_timeout(-1));
            } else {
                return null;
            }
        }
    }

    @Override
    public String createTicket(TicketInfo info, long expiredTime) {
        return _cacheManager.getProxy(0).createTicket(info.demark(), (int) expiredTime);
    }

    @Override
    public long verifyTicket(String ticket, List<Integer> types) {
        long result = -1;
        if (TicketUtil.isSStringValid(ticket)) {
            String lTicket = TicketUtil.sTicket2LTicket(ticket);
            if (TicketUtil.isCheckSumValid(lTicket)) {
                TicketData data = getTicketData(ticket);
                if (data != null) {
                    int type = data.getType();
                    if (types.contains(type)) {
                        result = data.getId();
                    }
                }
            }
        }

        return result;
    }

    @Override
    public boolean destroyTicket(String ticket) {
        return _cacheManager.getProxy(0).destroyTicket(ticket);
    }

    @Override
    public TicketInfo getTicketInfo(String ticket) {
        TicketInfo result = null;
        TicketData data = getTicketData(ticket);
        if (data != null) {
            result = new TicketInfo();
            result.set_id(data.getId());
            result.set_realId(data.getRealId());
            result.set_type(data.getType());
            result.set_ip(data.getIp());
        }
        return result;
    }

    @Override
    public boolean destroyTicketById(long id, List<Integer> types) {
        return _cacheManager.getProxy(0).destroyTicketById(id, MathUtil.list2array(types));
    }

    @Override
    public boolean destroyTicketById(long id, int type) {
        List<Integer> types = new ArrayList<Integer>();
        types.add(type);
        return destroyTicketById(id, types);
    }

    private TicketData getTicketData(String ticket) {
        TicketData result = null;
        byte[] data = null;
        try {
            data = _cacheClient.getAndIgnoreMiss(ticket);
        } catch (TripodException e) {
            Oce.getLogger().error("TicketImpl.getTicketData ticket: " //NL
                    + ticket + " TripodException msg:" + e.getMessage());
        }
        if (data != null && data.length > 0) {
            try {
                result = TicketData.parseFrom(data);
            } catch (InvalidProtocolBufferException e) {
                Oce.getLogger().error("TicketImpl.getTicketData ticket: " //NL
                        + ticket + " InvalidProtocolBuf" + e.getMessage());
            }
        }
        return result;
    }
}

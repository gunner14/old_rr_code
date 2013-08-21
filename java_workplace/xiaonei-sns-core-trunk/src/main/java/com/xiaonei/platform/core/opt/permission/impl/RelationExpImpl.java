package com.xiaonei.platform.core.opt.permission.impl;

import java.util.ArrayList;
import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.buddy.BuddyRelation;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.NetworkBigObjectN;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.xiaonei.platform.core.model.Network;
import com.xiaonei.platform.core.opt.jdbc.dao.GoingDAO;
import com.xiaonei.platform.core.opt.permission.RelationExp;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.opt.permission.source.Status;

public class RelationExpImpl implements RelationExp {
	private static Logger logger = LogManager.getLogger(RelationExpImpl.class);

	private static RelationExp rep = new RelationExpImpl();

	public static RelationExp getInstance() {
		return rep;
	}

	public boolean isBlock(BuddyRelation guesterToOwner) {
		return guesterToOwner.isBlock();
	}

	public boolean isBlocked(BuddyRelation guesterToOwner) {
		return guesterToOwner.isBlocked();
	}

	public boolean isRequestFriend(BuddyRelation guesterToOwner) {
		// return guesterToOwner.getDisc() == RelationDef.uuRequestGuest ? true
		// : false;
		return guesterToOwner.isApply();
	}

	public boolean isRequestFriended(BuddyRelation guesterToOwner) {
		// return guesterToOwner.getDisc() == RelationDef.uuGuestRequest ? true
		// : false;
		return guesterToOwner.isApplied();
	}

	public boolean isFriend(BuddyRelation guesterToOwner) {
		return guesterToOwner.isFriend();
	}

	public boolean isSamer(int guester, int owner) {
		return (guester == owner) ? true : false;

	}

	public boolean isSameNet(Source guester, Source source) {
		Status guest = (Status) guester;
		Status owner = (Status) source;
		return (guest.getStage() == owner.getStage() && guest.getNet() == owner
				.getNet()) ? true : false;

	}

	public boolean isSendConfirmed(int guester, int owner) {
		return false;
	}

	public boolean isSameStage(Source guester, Source owner) {
		Status gst = (Status) guester;
		Status ost = (Status) owner;
		return (gst.getStage() == ost.getStage()) ? true : false;

	}

	public boolean isGouYinGuo(Source guester, Source owner) {
		try {
			boolean bl = GoingDAO.getInstance().isGouYinGou(owner.getUserId(),
					guester.getUserId());
			return bl;
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return false;
	}

	/*
	 * 最终该实现放入 RelationLogic中 (non-Javadoc)
	 */
	public int getRelationPath(Source guester, Source owner) {
		Status gst = (Status) guester;
		Status ost = (Status) owner;
		if (gst == null || gst.getId() == 0) {
			return RelationDef.uuNoPath;
		}

		if (gst.getUserId() == ost.getUserId()) {
			return RelationDef.uuSelf;
		}

		BuddyRelation rl1 = null;
		try {
			rl1 = AdapterFactory.getBuddyCoreAdapter().getRelation(
					gst.getUserId(), ost.getUserId());

		} catch (Exception e) {
			rl1 = null;
		}

		if (rl1 != null) {
			if (rl1.isFriend())
				return RelationDef.uuFriend;
			if (rl1.isApplied())
				return RelationDef.uuGuestRequest;
			if (rl1.isBlocked())
				return RelationDef.uuBlocked;
		}
		if (gst.getStage() == Stage.STAGE_HEIGHSCHOOL
				&& ost.getStage() == Stage.STAGE_HEIGHSCHOOL) {
			logger.debug("Same Stage for owner:" + ost.getUserId()
					+ " and gst:" + gst.getUserId());
			return RelationDef.uuSchoolmate;
		}

		// new Service for same Netowrk and University compare
		NetworkBigObjectN gstN = gst.getNetbig() != null ?gst.getNetbig() : AdapterFactory.getNetworkAdapter()
				.getNetworkBigObjectN(gst.getUserId());
		NetworkBigObjectN ostN = ost.getNetbig() != null ?ost.getNetbig() : AdapterFactory.getNetworkAdapter()
				.getNetworkBigObjectN(ost.getUserId());
		if (gstN != null && ostN != null) {
			// fetch gstN's all network
			List<Network> gstNetworks = new ArrayList<Network>();
			if (gstN.getNetworks() != null && gstN.getNetworks().size() > 0) {
				for (mop.hi.oce.domain.network.Network net : gstN.getNetworks()) {
					Network network = new Network();
					network.setId(net.getId());
					network.setName(net.getNetworkName());
					gstNetworks.add(network);
				}
			}
			// fetch ostN's all network
			List<Network> ostNetworks = new ArrayList<Network>();
			if (ostN.getNetworks() != null && ostN.getNetworks().size() > 0) {
				for (mop.hi.oce.domain.network.Network net : ostN.getNetworks()) {
					Network network = new Network();
					network.setId(net.getId());
					network.setName(net.getNetworkName());
					ostNetworks.add(network);
				}
			}
			// compare has the same network
			List<Network> gstNetworksCopy = new ArrayList<Network>();
			gstNetworksCopy.addAll(gstNetworks);
			gstNetworksCopy.retainAll(ostNetworks);// 用jdk内建比较方法
			if (!gstNetworksCopy.isEmpty()) {
				logger.debug("Same Network for owner:" + ost.getUserId()
						+ " and gst:" + gst.getUserId());
				for (Network network : gstNetworksCopy) {
					logger.debug("ID:" + network.getId());
					logger.debug("NAME:" + network.getName());
				}
				return RelationDef.uuSchoolmate;
			}
			gstNetworks.clear();
			ostNetworks.clear();
			if (ostN.getCollages() != null)
				for (CollegeInfo ci : ostN.getCollages()) {
					ostNetworks.add(convert(ci));
				}
			if (gstN.getCollages() != null)
				for (CollegeInfo ci : gstN.getCollages()) {
					gstNetworks.add(convert(ci));
				}
			if (ostN.getElementaryschool() != null)
				ostNetworks.add(convert(ostN.getElementaryschool()));
			if (gstN.getElementaryschool() != null)
				gstNetworks.add(convert(gstN.getElementaryschool()));

			if (ostN.getHighschools() != null)
				for (HighSchoolInfo hsi : ostN.getHighschools()) {
					ostNetworks.add(convert(hsi));
				}
			if (gstN.getHighschools() != null)
				for (HighSchoolInfo hsi : gstN.getHighschools()) {
					gstNetworks.add(convert(hsi));
				}
			if (ostN.getUnivs() != null)
				for (UniversityInfo ui : ostN.getUnivs()) {
					ostNetworks.add(convert(ui));
				}
			if (gstN.getUnivs() != null)
				for (UniversityInfo ui : gstN.getUnivs()) {
					gstNetworks.add(convert(ui));
				}
			if (ostN.getWorkspaces() != null)
				for (WorkplaceInfo wi : ostN.getWorkspaces()) {
					ostNetworks.add(convert(wi));
				}
			if (gstN.getWorkspaces() != null)
				for (WorkplaceInfo wi : gstN.getWorkspaces()) {
					gstNetworks.add(convert(wi));
				}
			if (ostN.getJuniorhighschool() != null)
				ostNetworks.add(convert(ostN.getJuniorhighschool()));

			if (gstN.getJuniorhighschool() != null)
				gstNetworks.add(convert(gstN.getJuniorhighschool()));

			gstNetworks.retainAll(ostNetworks);
			if (!gstNetworks.isEmpty()) {
				logger.debug("Same Information for owner:" + ost.getUserId()
						+ " and gst:" + gst.getUserId());
				for (Network network : gstNetworks) {
					logger.debug("ID:" + network.getId());
					logger.debug("NAME:" + network.getName());
				}
				return RelationDef.uuSchoolmate;
			}

		}
		if (rl1 == null)
			return RelationDef.uuNoPath;
		else if (rl1.isApply()) {
			return RelationDef.uuRequestGuest;
		} else if (rl1.isBlock()) {
			return RelationDef.uuBlock;
		} else {
			return RelationDef.uuNoPath;
		}

	}

	private Network convert(CollegeInfo coll) {
		Network net = new Network();
		net.setId(coll.getCollegeId());
		net.setName(coll.getCollegeName());
		return net;
	}

	private Network convert(ElementarySchoolInfo elem) {
		Network net = new Network();
		net.setId(elem.getElementarySchoolId());
		net.setName(elem.getElementarySchoolName());
		return net;
	}

	private Network convert(JuniorHighSchoolInfo juni) {
		Network net = new Network();
		net.setId(juni.getJunHighentarySchoolId());
		net.setName(juni.getJunHighentarySchoolName());
		return net;
	}

	private Network convert(HighSchoolInfo high) {
		Network net = new Network();
		net.setId(high.getHighSchoolId());
		net.setName(high.getHighSchoolName());
		return net;
	}

	private Network convert(UniversityInfo univ) {
		Network net = new Network();
		net.setId(univ.getUniversityId());
		net.setName(univ.getUniversityName());
		return net;
	}

	private Network convert(WorkplaceInfo work) {
		Network net = new Network();
		net.setId(work.getWorkplaceId());
		net.setName(work.getWorkplaceName());
		return net;
	}
}

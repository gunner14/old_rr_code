package com.xiaonei.reg.guide.flows.xfivebrown.controllers;

import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.dodoyo.friends.relation.factory.RelationModelFactory;
import com.dodoyo.friends.relation.logicmodel.IRelationModel;
import com.dodoyo.friends.relationfriend.logic.IRelation;
import com.dodoyo.friends.utils.FriendsUtil;
import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationFriend;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

@LoginRequired
@Path("pvbrown")
@IgnoreUserStatusImportedValidation
public class BrownPreviewInitController {

	private static final String SUCC_DEFAULT = "/views/guide/flowxfivedark/preview_dark.jsp";
	private static final String SUCC_PVZ = "/views/guide/flowxfivedarkpvz/preview_dark.jsp";
	private static final String SKIP_DEFAULT = "f:lead";
	private final static int thisStep = StepStatusLogic.STEP_PREVIEW;
	private static final int RESULT_LIMIT = 24;

	@Post
	@Get
	public String index(Invocation inv) {

		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		if (request == null || response == null) {
			GuideLogger.printLog(" request or response null " + (request == null) + (response == null), GuideLogger.ERROR);
		} else if (host == null) {
			GuideLogger.printLog(" host null ", GuideLogger.ERROR);
		} else {
			try {
				GuideTimeCost cost = GuideTimeCost.begin();
				String skip = GuideXFiveRequestUtil.getParamString(request, "skip", host.getId());
				cost.endStep();
				if (MyStringUtils.equals(skip, "1")) {
					skip(host);
					cost.endStep();
					cost.endFinally();
					return SKIP_DEFAULT;
				}				
				List<GuideIRelation> mayFriends = getFriendsListForLead(inv,host);
				cost.endStep();
				if (mayFriends == null || countFriends(mayFriends, host) <= 0) {
					GuideLogger.printLog(" host:" + host.getId());
					skip(host);
					cost.endStep();
					cost.endFinally();
					return SKIP_DEFAULT;
				} else {
					GuideLogger.printLog(" host:" + host.getId() + " friends:"+ countFriends(mayFriends, host));
					cost.endStep();
					cost.endFinally();
					
					String fromsource = RegFromSourceLogic.getFromSource(host);
					int type = StepProcessLogic.getInstance().getGuideUserType(host,fromsource);
					if(type == 13&&fromsource.contains(":PVZ:")){
						return SUCC_PVZ;
					}
					
					return SUCC_DEFAULT;
				}
			} catch (Exception e) {
				GuideLogger.printLog(" host:" + host.getId() + " e:" + e.toString(), GuideLogger.ERROR);
				e.printStackTrace();
			}
		}
		return SUCC_DEFAULT;
	}

	private void skip(User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		GuideLogger.printLog(" host:" + host.getId() + " skip:" + thisStep);
		StepStatusLogic.updateStepStatus(host.getId(), thisStep);
		cost.endFinally();

	}

	private List<GuideIRelation> getFriendsListForLead(Invocation inv, User host) {		
		List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(RESULT_LIMIT, host, -1);
		if (list == null)
			list = new ArrayList<GuideIRelation>();

		StringBuilder sb = new StringBuilder();
		int cnt = 0;
		int innercnt = 0;
		for (GuideIRelation ir : list) {
			List<GuideIRelationFriend> aa = ir.getList();
			for (GuideIRelationFriend f : aa) {
				if (innercnt++ >= RESULT_LIMIT) {
					break;
				}
				sb.append(f.getId());
				sb.append(",");
				cnt++;
			}
		}
		GuideLogger.printLog(" host:" + host.getId() + " cnt:" + cnt + " "
				+ "f:[" + sb.toString() + "]");

		String json = GuideXFiveRelationsPreviewLogic.getInstance()
				.getJSONForFriendSelector(list);
		GuideLogger.printLog(" host:" + host.getId() + " get:"
				+ countFriendsMore(list, host));
		inv.addModel("lead_may_known", json);

		return list;

	}

	private int countFriends(List<GuideIRelation> list, User host) {
		int ret = 0;
		if (list != null) {
			for (GuideIRelation relation : list) {
				ret += relation.getCount();
			}
		}
		return ret;
	}

	private int countFriendsMore(List<GuideIRelation> list, User host) {
		int ret = 0;
		if (list != null) {
			for (GuideIRelation relation : list) {
				GuideLogger.printLog(" host:" + host.getId() + " i:"
						+ relation.getTypeName() + " c:" + relation.getCount());
				ret += relation.getCount();
			}
		}
		return ret;
	}

	@SuppressWarnings("unused")
	private List<IRelation> getFriendsList(Invocation inv, User host) {
		GuideTimeCost cost = GuideTimeCost.begin();
		List<IRelation> mayFriends = null;
		User user = host;
		try {
			int amount = 49;
			long beginMayKnown = LogTimeUtils.logBegin();
			List<IRelationModel> list = new ArrayList<IRelationModel>();
			IRelationModel workerModel = RelationModelFactory.create(user,
					amount, RelationModelFactory.COWORKER_RELATION_MODEL);
			IRelationModel universityModel = RelationModelFactory.create(user,
					amount, RelationModelFactory.UNIVERSITY_RELATION_MODEL);
			IRelationModel collegeModel = RelationModelFactory.create(user,
					amount, RelationModelFactory.COLLEGE_RELATION_MODEL);
			IRelationModel highModel = RelationModelFactory.create(user,
					amount, RelationModelFactory.HIGH_RELATION_MODEL);
			IRelationModel juniorModel = RelationModelFactory.create(user,
					amount, RelationModelFactory.JUNIOR_RELATION_MODEL);
			IRelationModel elementaryModel = RelationModelFactory.create(user,
					amount, RelationModelFactory.ELEMENTARY_RELATION_MODEL);
			// IRelationModel locationModel = RelationModelFactory.create(user,
			// amount, RelationModelFactory.LOCATION_RELATION_MODEL);

			list.add(workerModel);
			list.add(universityModel);
			list.add(collegeModel);
			list.add(highModel);
			list.add(juniorModel);
			list.add(elementaryModel);
			// list.add(locationModel);
			mayFriends = FriendsUtil.findPeople(user, list);
			GuideLogger.printLog(" host:" + host.getId() + " find:"
					+ mayFriends.size());
			// inv.addModel("lead_company_and_school", mayFriends);
			// 拼json串
			// String json =
			// GuideUtils.getInstance().getJSONPeopleMayKnowForGuide(mayFriends);
			String json = GuideUtil.getJSONForFriendSelector(mayFriends);

			inv.addModel("lead_may_known", json);
			LogTimeUtils.logEnd(beginMayKnown, "EndMayKnown_preview");
		} catch (Exception e) {
			GuideLogger.printLog(
					" host:" + host.getId() + " e:" + e.toString(),
					GuideLogger.ERROR);
			e.printStackTrace();
		}
		cost.endFinally();
		return mayFriends;
	}

}

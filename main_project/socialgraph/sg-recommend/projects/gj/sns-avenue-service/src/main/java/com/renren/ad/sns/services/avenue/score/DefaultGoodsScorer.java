package com.renren.ad.sns.services.avenue.score;

import static com.renren.ad.sns.services.avenue.util.ValueNorm.getNormValue;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.services.avenue.GoodsMeta;
import com.renren.ad.sns.services.avenue.Scorer;

public class DefaultGoodsScorer implements Scorer<GoodsMeta> {

	private GoodsScoreParam scoreParam = new GoodsScoreParam();

	private Map<Long, Spammer> spammers = new HashMap<Long, Spammer>();

	private Map<Long, Integer> id2friendCount = new HashMap<Long, Integer>();

	private Set<Long> starUsers = new HashSet<Long>();

	public static final SimpleDateFormat DF = new SimpleDateFormat("yyMMddhhmm");

	private static final Log LOG = LogFactory.getLog(DefaultGoodsScorer.class);
	
	private Set<Long> goodsHidden=new HashSet<Long>();
	
	private Set<Long> operationUsers= new HashSet<Long>(); // 运营人员的数据，在紧急情况下，可以把他们的商品进行展示

	@Override
	public double score(GoodsMeta goodsMeta) {
		double score = 0;
		StringBuffer scoreDetails = new StringBuffer(goodsMeta.getGoodsId()
				+ " [");

		int isStar = 0;
		if (starUsers != null && starUsers.contains(goodsMeta.getOwnerId())) {
			isStar = 1;
		}

		// 1. goods publish time
		String publishTime = "" + goodsMeta.getGoodsId() / 1000000000L;
		long publishInterval = 0;
		try {
			publishInterval = (System.currentTimeMillis() - DF.parse(
					publishTime).getTime())
					/ (60 * 1000);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		double scorePubTime = 0;
		if (isStar == 0) {
			scorePubTime = scoreParam.getwGoodsPublishTime()
					* getNormValue(scoreParam.gettGoodsPublishTime(),
							publishInterval);
		} else {
			scorePubTime = scoreParam.getwStarUserGoodsPublishTime()
					* getNormValue(scoreParam.gettStarUserGoodsPublishTime(),
							publishInterval);
		}
		// 三天外商品的统一降权
		if(publishInterval > GoodsScoreParam.THREE_DAYS){
			scorePubTime += scoreParam.getGoodsPublishTimeOut3day();
		}
		
		// 五天外商品的统一降权
		if(publishInterval > GoodsScoreParam.FIVE_DAYS){
			scorePubTime += scoreParam.getGoodsPublishTimeOut5day();
		}
		
		// 七天外商品的统一降权
		if(publishInterval > GoodsScoreParam.SERVEN_DAYS){
			scorePubTime += scoreParam.getGoodsPublishTimeOut7day();
		}
		
		// 2. goods real like count
		double scoreNumLike = scoreParam.getwGoodsNumLike()
				* getNormValue(scoreParam.gettGoodsNumLike(), goodsMeta
						.getRealLikeCount());
		if(goodsMeta.getLikeCount()==0&&isStar==0){
			scoreNumLike+=scoreParam.getZeroLikeWeight();
		}
		// 3. goods share count
		double scoreNumShare = scoreParam.getwGoodsNumShare()
				* getNormValue(scoreParam.gettGoodsNumShare(), goodsMeta
						.getShareCount());

		// 4. goods source, src=1为淘宝，src=0其他
		double scoreTbSrc = 0;
		if(scoreParam.gettGoodsTaobaoSrc() == 1){
		    scoreTbSrc = scoreParam.getwGoodsTaobaoSrc()
            * getNormValue(scoreParam.gettGoodsTaobaoSrc(), goodsMeta
                    .getSource());
		}

		// 5. user is spammer
		int isSpammer = 0;
		if (spammers != null && spammers.containsKey(goodsMeta.getOwnerId())) {
			isSpammer = 1;
		}
		double scoreIsSpam = scoreParam.getwUserIsSpam()
				* getNormValue(scoreParam.gettUserIsSpam(), isSpammer);

		// 6. user default head pic
		int isDefHead = 0;
		if (goodsMeta.isDefaultOwnerPic()) {
			isDefHead = 1;
		}
		double scoreIsDefHead = scoreParam.getwUserDefaultHead()
				* getNormValue(scoreParam.gettUserDefaultHead(), isDefHead);

		// 7. user credible(黑名单，降权)
		double scoreCredible = 0;
		if (goodsMeta.getCredible() == -1) {
			scoreCredible = scoreParam.getwUserCredible()
					* getNormValue(scoreParam.gettUserCredible(), goodsMeta
							.getCredible());
		}

		// 8. user reputation
		double scoreReputation = scoreParam.getwUserReputation()
				* getNormValue(scoreParam.gettUserReputation(), goodsMeta
						.getReputation());
		if(goodsMeta!=null&goodsMeta.getReputation()<=0.2){
			if(goodsMeta.isDefaultReputation()){
				scoreReputation-=50;
			}
			scoreReputation-=10;
		}
		// 9. user renren friend
		int numFriend = 0;
		if (id2friendCount != null
				&& id2friendCount.containsKey(goodsMeta.getOwnerId())) {
			numFriend = id2friendCount.get(goodsMeta.getOwnerId());
		}
		double scoreNumFriend = scoreParam.getwUserNumFriend()
				* getNormValue(scoreParam.gettUserNumFriend(), numFriend);

		// 10. user is guangjie star
		double scoreStar = scoreParam.getwUserIsStar()
				* getNormValue(scoreParam.gettUserIsStar(), isStar);

		// 11. 达人近3天发布商品的额外加分
		double scoreStarUserPubTime = 0;
		if (isStar == 1
				&& publishInterval > 0
				&& publishInterval < scoreParam
						.gettStarUserAdditionalPublishTime()) {
			long deltaTime = (long) scoreParam
					.gettStarUserAdditionalPublishTime()
					- publishInterval;
			scoreStarUserPubTime = scoreParam
					.getwStarUserAdditionalPublishTime()
					* getNormValue(scoreParam
							.gettStarUserAdditionalPublishTime(), deltaTime);
		}
		
		
		//12,人工隐藏的商品降权
		double hiddenGoodsScore = 0;
		if(goodsHidden.contains(goodsMeta.getGoodsId())){
			hiddenGoodsScore = scoreParam.getHiddenGoodsWeigh();
		}
		
		//13,运营编辑的用户加分
		double operationUsersScore=0;
		if(scoreParam.getIsSupportOperationUser()==1){
			operationUsersScore=scoreParam.getOperationUserScore();
		}
		score += scorePubTime + scoreNumLike + scoreNumShare + scoreTbSrc
				+ scoreIsSpam + scoreIsDefHead + scoreCredible
				+ scoreReputation + scoreNumFriend + scoreStar
				+ scoreStarUserPubTime+hiddenGoodsScore+operationUsersScore+200;

		if (LOG.isDebugEnabled()) {
			scoreDetails.append("Goods publish time:" + scorePubTime + ", ");
			scoreDetails.append("Goods like count:" + scoreNumLike + ", ");
			scoreDetails.append("Goods share count:" + scoreNumShare + ", ");
			scoreDetails.append("Goods taobao src:" + scoreTbSrc + ", ");
			scoreDetails.append("User is spam:" + scoreIsSpam + ", ");
			scoreDetails.append("User default head:" + scoreIsDefHead + ", ");
			scoreDetails.append("User credible:" + scoreCredible + ", ");
			scoreDetails.append("User reputation:" + scoreReputation + ", ");
			scoreDetails.append("User renren friend count:" + scoreNumFriend
					+ ", ");
			scoreDetails.append("User is star:" + scoreStar + ", ");
			scoreDetails.append("User star publish time::"
					+ scoreStarUserPubTime + "]");
			scoreDetails.append(" score:" + score);
			LOG.debug(scoreDetails.toString());
			//System.out.println(scoreDetails.toString());
		}
		
		return score;
	}

	public void setScoreParam(GoodsScoreParam scoreParam) {
		if (scoreParam != null) {
			this.scoreParam = scoreParam;
		}
	}

	public void setSpammers(Map<Long, Spammer> spammers) {
		if (spammers != null) {
			this.spammers = spammers;
		}
	}

	public void setId2friendCount(Map<Long, Integer> id2friendCount) {
		if (id2friendCount != null) {
			this.id2friendCount = id2friendCount;
		}
	}

	public void setId2friendCount2(Map<Integer, Integer> id2friendCount) {
		if (id2friendCount != null) {
			for (int id : id2friendCount.keySet()) {
				this.id2friendCount.put(new Long(id), id2friendCount.get(id));
			}
		}
	}

	public void setGoodsHidden(Set<Long> goodsHidden) {
		this.goodsHidden = goodsHidden;
	}

	public void setStarUsers(Set<Long> starUsers) {
		this.starUsers = starUsers;
	}

	public void setOperationUsers(Set<Long> operationUsers) {
		this.operationUsers = operationUsers;
	}
	

}

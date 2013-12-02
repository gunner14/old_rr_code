package com.renren.ad.sns.services.uranus;

import static org.junit.Assert.*;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.BeforeClass;
import org.junit.Test;

import com.google.code.morphia.query.Query;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.data.Spammer;
import com.renren.ad.sns.services.uranus.CategoryCandidatePicker.LinkComparator;

public class SimilarityTagList2CandidatePickerTest {

	private static final Log LOG = LogFactory
			.getLog(CategoryCandidatePicker.class);
	
	/* 商品到tag的map */
    private static Map<Long, List<Long>> goods2tagList;
    private static Map<Long, CandidateList> tag2Candidate;
    private static Map<Long, Integer> goods2category;

    private final static Object tag2CandidateLock = new byte[0];

    private final static Object goods2tagListLock = new byte[0];
    
    private static List<Goods> loadGoods(String filename) {
        long beg = System.currentTimeMillis();
        List<Goods> all = new ArrayList<Goods>();
        Scanner cin;
        try {
            cin = new Scanner(new FileInputStream(filename));
            while (cin.hasNext()) {
                String line = cin.nextLine();
                try {
                    String[] tmps = line.split(",");
                    Goods goods = new Goods();
                    if (tmps.length >= 7) {
                        goods.setGoodsId(Long.parseLong(tmps[0]));
                        goods.setOwnerId(Long.parseLong(tmps[1]));
                        goods.setCategoryId(Integer.parseInt(tmps[2]));
                        goods.setPrice(Integer.parseInt(tmps[3]));
                        goods.setShareCount(Integer.parseInt(tmps[4]));
                        goods.setLikeCount(Integer.parseInt(tmps[6]));
                        goods.setLink(tmps[7]);
                        // parse tag list
                        List<Long> tags = new ArrayList<Long>();
                        if (tmps.length == 7 + 2) {
                            String stags = tmps[tmps.length - 1];
                            String[] ttags = stags.split(":");
                            for (String stag : ttags) {
                                try {
                                    long tag = Long.parseLong(stag);
                                    tags.add(tag);
                                } catch (Exception e) {
                                    LOG.error("parse tag list error " + stags, e);
                                }
                            }
                        }
                        goods.setTagList(tags);
                        all.add(goods);
                    }
                } catch (Exception e) {
                    LOG.error("Parse U2C Exception : " + line, e);
                }
            }
            cin.close();
        } catch (FileNotFoundException e) {
            LOG.error("load new goods.dat error!", e);
        }
        Collections.sort(all, new LinkComparator());
        String last = "";
        String link = last;
        List<Goods> res = new ArrayList<Goods>();
        for (Goods g : all) {
            link = g.getLink();
            if (last.compareTo(link) != 0) {
                res.add(g);
            }
            last = link;
        }
        long time = System.currentTimeMillis() - beg;
        LOG.info("loaded " + res.size() + " goods over , elapsed " + time + " ms!");
        return res;
    }

    private static double score(Goods goods) {
        double point = 0;
        long date = goods.getGoodsId() / 1000000000l;
        double delta = 0.20;
        if (goods.getOwnerId() == 443215821) {
            delta = 0.15;
        }
        point = Math.log(1.0
                + Math.log(date)
                // * 0.0001
                + Math.log(1.0 + Math.log(1.0 + goods.getLikeCount() * delta
                        + goods.getShareCount() * 0.05)));
        if (goods.getSource() == 1) {
            point -= 0.000075;
        }
        return point;
    }

    static class LinkComparator implements Comparator<Goods> {

        @Override
        public int compare(Goods g1, Goods g2) {
            String link1 = g1.getLink();
            if (link1 == null) {
                link1 = "";
                g1.setLink("");
            }
            String link2 = g2.getLink();
            if (link2 == null) {
                link2 = "";
                g2.setLink("");
            }

            int res = link1.compareTo(link2);
            if (res == 0) {
                res = g1.getGoodsId() - g2.getGoodsId() > 0 ? 1 : -1;
            }
            return res;
        }
    }
    
	private static void updateCandidate(String filename) {
        long t_beg = System.currentTimeMillis();
        //Map<Long, Spammer> spammers = loadSpammer();
        List<Goods> newGoods = loadGoods(filename);
        tag2Candidate = new HashMap<Long, CandidateList>();
        goods2tagList = new HashMap<Long, List<Long>>();
        goods2category = new HashMap<Long, Integer>();
        Map<Long, CandidateList> tmpTag2Candidate = new HashMap<Long, CandidateList>();
        Map<Long, List<Long>> tmpGoods2tagList = new HashMap<Long, List<Long>>();
        Map<Long, Integer> tmpGoods2category = new HashMap<Long, Integer>();

        for (Goods g : newGoods) {
            long ownerId = g.getOwnerId();
            long goodsId = g.getGoodsId();
            double weight = score(g);
            List<Long> tags = g.getTagList();
            int catId = g.getCategoryId();

            if (g.getLink().startsWith(CandidatesFilterUtil.BAN_URL)) {
                continue;
            }

            tmpGoods2tagList.put(goodsId, tags);

            Candidate candidate = new Candidate(g.getGoodsId(), weight,
                    CandidateType.CategoryCandidatePicker);
            for (Long tag : tags) {
                CandidateList candidateList = null;
                if (tmpTag2Candidate.containsKey(tag)) {
                    candidateList = tmpTag2Candidate.get(tag);
                } else {
                    candidateList = new CandidateList(5000);
                }
                candidateList.add(candidate);
                tmpTag2Candidate.put(tag, candidateList);
            }
            
            tmpGoods2category.put(goodsId, catId);
        }

        if (tmpTag2Candidate.size() > 0) {
            synchronized (tag2CandidateLock) {
                tag2Candidate = tmpTag2Candidate;
            }
            LOG.info("update candidate over , tag2Candidate size = " + tag2Candidate.size()
                    + ", elapse " + (System.currentTimeMillis() - t_beg) + " ms!");
        }
        if (tmpGoods2tagList.size() > 0) {
            synchronized (goods2tagListLock) {
                goods2tagList = tmpGoods2tagList;
            }
            LOG.info("update goods2tagList over , size = " + goods2tagList.size() + ", elapse "
                    + (System.currentTimeMillis() - t_beg) + " ms!");
        }

        if(tmpGoods2category.size() > 0){
            goods2category = tmpGoods2category;
        }
    }
	
	private SimilarityTagList2CandidatePicker picker = new SimilarityTagList2CandidatePicker();
	
	@BeforeClass
    public static void init() {
		updateCandidate("./test-data/goods.dat");
    }
	


	@Test
	public void testPick() {
		long gid = 1206011450014283166L;
		
		List<Long> tagList = this.goods2tagList.get(gid);
		
		assertTrue(goods2tagList.containsKey(gid));
		//assertEquals(tag2Candidate.get(10272L).size(), 3);
		
		List<Candidate> result = picker.pickSync(gid, tagList, tag2Candidate, goods2category);
		assertEquals(result.size(), 5);
		assertEquals(result.get(0).getGid(), 1206011450014283162L);
	}

}

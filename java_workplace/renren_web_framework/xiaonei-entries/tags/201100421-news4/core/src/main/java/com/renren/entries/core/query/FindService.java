//package com.renren.entries.core.query;
//
//import java.util.ArrayList;
//import java.util.Collections;
//import java.util.HashMap;
//import java.util.Iterator;
//import java.util.List;
//import java.util.Map;
//import java.util.Map.Entry;
//import java.util.Set;
//
//import org.springframework.beans.factory.annotation.Autowired;
//import org.springframework.stereotype.Service;
//
//import com.renren.entries.core.dao.AtDAO;
//import com.renren.entries.core.dao.AuthorDAO;
//import com.renren.entries.core.dao.BodyDAO;
//import com.renren.entries.core.dao.CreateTimeDAO;
//import com.renren.entries.core.dao.InfoDAO;
//import com.renren.entries.core.dao.SeqDAO;
//import com.renren.entries.core.dao.TagDAO;
//import com.renren.entries.core.exception.EntryException;
//import com.renren.entries.core.util.IndexComparator;
//import com.renren.entries.core.util.TableUtil;
//import com.renren.entries.core.util.TimeUtil;
//import com.renren.entries.model.Index;
//import com.renren.entries.model.EntryPage;
//
///**
// * 根据query进行查询，外层的空格用于取交集，内层的空格用于取并集
// * 
// * @author yanghe.liang@opi-corp.com
// */
//@Service
//public class FindService {
//
//    public static final int ROLL_TIME = 2;
//
//    public static final int ROLL_MAX_LIMIT = 500;
//
//    @Autowired
//    protected AtDAO atDAO;
//
//    @Autowired
//    protected AuthorDAO authorDAO;
//
//    @Autowired
//    protected InfoDAO infoDAO;
//
//    @Autowired
//    protected BodyDAO bodyDAO;
//
//    @Autowired
//    protected CreateTimeDAO createTimeDAO;
//
//    @Autowired
//    protected TagDAO tagDAO;
//
//    @Autowired
//    protected SeqDAO seqDAO;
//
//    /**
//     * 根据query进行查询，外层的and用于取交集，内层的or用于取并集 例如: query =
//     * "tag:(123 or 456 or 678) and create_time:232134-232144"
//     * 
//     * @param query
//     * @return
//     * @throws EntryException
//     */
//    public EntryPage find(String query, long cursor, int limit) {
//        if (cursor <= 0) {
//            cursor = Long.MAX_VALUE;
//        }
//
//        RootNode root = parse(query);
//        int maxOr = root.getMaxOr();
//        int maxAnd = root.getMaxAnd();
//
//        // 没有并集，也没有交集
//        if (maxOr == 1 && maxAnd == 1) {
//            return single(root, cursor, limit);
//        }
//
//        // 有并集，没有交集
//        if (maxOr > 1 && maxAnd == 1) {
//            return onlyOr(root, cursor, limit);
//        }
//
//        // 没有并集，有交集
//        if (maxOr == 1 && maxAnd > 1) {
//            return onlyAnd(root, cursor, limit);
//        }
//
//        // 既有并集，又有交集
//        return complete(root, cursor, limit);
//    }
//
//    /**
//     * 构造query树
//     * 
//     * @param query
//     * @return
//     * @throws EntryException
//     */
//    private RootNode parse(String query)  {
//        RootNode root = new RootNode();
//        String[] andTerms = query.split(" AND ");
//        for (String andTerm : andTerms) {
//            andTerm = andTerm.trim();
//            String[] kv = andTerm.split(":");
//            String key = kv[0];
//            if (!TableUtil.contain(key)) {
//                throw new EntryException("query中含有不合法的key");
//            }
//            String value = kv[1];
//            AndNode andNode = new AndNode(key);
//            if (value.startsWith("(")) { // 去掉外围括号
//                int len = value.length();
//                value = value.substring(1, len - 1);
//            }
//            String[] orTerms = value.split(" OR ");
//            for (String orTerm : orTerms) {
//                orTerm = orTerm.trim();
//                if (!TableUtil.CREATE_TIME.equals(key)) {
//                    andNode.addOrNode(Long.valueOf(orTerm));
//                } else {
//                    String[] be = orTerm.split("-");
//                    long begin = Long.valueOf(be[0]);
//                    long end = Long.valueOf(be[1]);
//                    // TODO 这段代码还没有测试
//                    if (!TimeUtil.inOneDay(begin, end)) {
//                        throw new EntryException(String.format(
//                                "time:%d and time:%d are not in one day.", begin, end));
//                    }
//                    andNode.addOrNode(begin, end);
//                }
//            }
//            root.addAndNode(andNode);
//        }
//        return root;
//    }
//
//    private List<Index> getIndexList(OrNode orNode, long cursor, int limit) {
//        String key = orNode.getField();
//        long value = orNode.getValue();
//        List<Index> indexList = new ArrayList<Index>();
//        if (TableUtil.TAG.equals(key)) {
//            indexList = tagDAO.gets(value, cursor, limit);
//        } else if (TableUtil.AUTHOR.equals(key)) {
//            indexList = authorDAO.gets(value, cursor, limit);
//        } else if (TableUtil.AT.equals(key)) {
//            indexList = atDAO.gets(value, cursor, limit);
//        } else if (TableUtil.CREATE_TIME.equals(key)) { // 时间是时间段，需要两个值
//            long value2 = orNode.getValue2();
//            indexList = createTimeDAO.gets(value, value2, cursor, limit);
//        }
//        return indexList;
//    }
//
//    private List<Index> getIndexListWithGids(OrNode orNode, long cursor, long[] gids) {
//        String key = orNode.getField();
//        long value = orNode.getValue();
//        List<Index> indexList = new ArrayList<Index>();
//        if (TableUtil.TAG.equals(key)) {
//            indexList = tagDAO.getsWithGids(value, cursor, gids);
//        } else if (TableUtil.AUTHOR.equals(key)) {
//            indexList = authorDAO.getsWithGids(value, cursor, gids);
//        } else if (TableUtil.AT.equals(key)) {
//            indexList = atDAO.getsWithGids(value, cursor, gids);
//        } else if (TableUtil.CREATE_TIME.equals(key)) {
//            // TODO 这段代码还没有测试
//            long value2 = orNode.getValue2();
//            indexList = createTimeDAO.getsWithGids(value, value2, cursor, gids);
//        }
//        return indexList;
//    }
//
//    private long[] trans(List<Index> indexList) {
//        long[] gids = new long[indexList.size()];
//        for (int i = 0; i < indexList.size(); i++) {
//            gids[i] = indexList.get(i).getEntryId();
//        }
//        return gids;
//    }
//
//    /**
//     * 单一索引查询，没有交集，也没有并集
//     * 
//     * @param root
//     * @param cursor
//     * @param limit
//     * @return
//     */
//    private EntryPage single(RootNode root, long cursor, int limit) {
//        OrNode orNode = root.getAndNodes().get(0).getOrNodes().get(0);
//        List<Index> indexList = getIndexList(orNode, cursor, limit);
//        long[] gids = new long[indexList.size()];
//        for (int i = 0; i < indexList.size(); i++) {
//            gids[i] = indexList.get(i).getEntryId();
//        }
//        cursor = indexList.size() > 0 ? indexList.get(indexList.size() - 1).getId() : 0;
//        return new EntryPage(cursor, gids);
//    }
//
//    /**
//     * 多索引查询，只有并集，没有交集
//     * 
//     * @param root
//     * @param cursor
//     * @param limit
//     * @return
//     */
//    private EntryPage onlyOr(RootNode root, long cursor, int limit) {
//        Map<Long, Index> map = new HashMap<Long, Index>();
//        List<OrNode> orNodeList = root.getAndNodes().get(0).getOrNodes();
//        for (OrNode orNode : orNodeList) {
//            List<Index> indexList = getIndexList(orNode, cursor, limit);
//            for (Index index : indexList) {
//                long gid = index.getEntryId();
//                Index old = map.get(gid);
//                if (old == null || index.getId() < old.getId()) {
//                    map.put(gid, index);
//                }
//            }
//        }
//        Set<Entry<Long, Index>> set = map.entrySet();
//        List<Index> result = new ArrayList<Index>();
//        for (Iterator<Entry<Long, Index>> itr = set.iterator(); itr.hasNext();) {
//            result.add(itr.next().getValue());
//        }
//
//        // 按indexId排序，即按时间排序
//        Collections.sort(result);
//
//        int min = Math.min(result.size(), limit);
//        long[] gids = new long[min];
//        for (int i = 0; i < min; i++) {
//            gids[i] = result.get(i).getEntryId();
//        }
//        cursor = min > 0 ? result.get(min - 1).getId() : 0;
//        return new EntryPage(cursor, gids);
//    }
//
//    /**
//     * 多索引查询，没有并集，只有交集
//     * 
//     * @param root
//     * @param cursor
//     * @param limit
//     * @return
//     */
//    private EntryPage onlyAnd(RootNode root, long cursor, int limit) {
//        List<Index> result = new ArrayList<Index>();
//        for (int i = 0; i < ROLL_TIME && result.size() < limit; i++) {
//            List<AndNode> andNodeList = root.getAndNodes();
//            OrNode first = andNodeList.get(0).getOrNodes().get(0);
//            List<Index> indexList = getIndexList(first, cursor, ROLL_MAX_LIMIT);
//            if (indexList.size() == 0) {
//                continue;
//            }
//            long[] gids = trans(indexList);
//            for (int j = 1; j < andNodeList.size(); j++) {
//                OrNode orNode = andNodeList.get(j).getOrNodes().get(0);
//                indexList = getIndexListWithGids(orNode, cursor, gids);
//                if (indexList.size() == 0) {
//                    break;
//                }
//                gids = trans(indexList);
//            }
//            if (indexList.size() == 0) {
//                continue;
//            }
//            cursor = indexList.get(indexList.size() - 1).getId();
//            result.addAll(indexList);
//        }
//
//        int min = Math.min(result.size(), limit);
//        long[] gids = new long[min];
//        for (int i = 0; i < min; i++) {
//            gids[i] = result.get(i).getEntryId();
//        }
//        cursor = min > 0 ? result.get(min - 1).getId() : 0;
//        return new EntryPage(cursor, gids);
//    }
//
//    /**
//     * 多索引查询，既有并集，又有交集
//     * 
//     * @param root
//     * @param cursor
//     * @param limit
//     * @return
//     */
//    //TODO 这个有点复杂，最后来搞
//    private EntryPage complete(RootNode root, long cursor, int limit) {
//        return null;
//    }
//
//    public static void main(String[] args) throws EntryException {
//        RootNode root = new FindService().parse("tag:123 and create_time:232134-232134");
//        root.dump();
//        System.out.println(root.getMaxAnd());
//        System.out.println(root.getMaxOr());
//        OrNode orNode = root.getAndNodes().get(0).getOrNodes().get(0);
//        System.out.println(orNode.getField());
//        System.out.println(orNode.getValue());
//        System.out.println(orNode.getValue2());
//        System.out.println("over");
//    }
//
//}

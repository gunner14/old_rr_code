package com.renren.entries.mysql.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.RowHandler;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.entries.model.Entry;
import com.renren.entries.mysql.dao.supports.HitRowMapper;
import com.renren.entries.mysql.model.Range;

/**
 * ${INDEX_TABLE}<br>
 * ${INDEX_NAME}<br>
 * ${INDEX_SELECT_COLUMNS}
 * 
 * @author qieqie.wang@gmail.com
 * @author yanghe.liang@renren-inc.com
 * 
 */
@DAO
public interface IndexDAO {

    /**
     * 增加一个索引条目
     * 
     * @param id 为该索引设置的一个主键
     * @param field 索引名称，比如"author"、"master"、"tag"等
     * @param fieldValue 索引值，比如author、master、tag等等
     * @param entryId 被索引的条目ID
     * @param entrySeq 被索引的条目Seq
     */
    @SQL("INSERT INTO ${INDEX_TABLE} (${INDEX_NAME}, entry_id, entry_seq)"
            + " VALUES (:fieldValue, :entryId, :entrySeq)")
    void add(//
            @SQLParam("field") String field, //
            @SQLParam("fieldValue") Object fieldValue, //
            @SQLParam("entryId") long entryId, //
            @SQLParam("entrySeq") long entrySeq //
    );

    /**
     * 删除给定的索引条目
     * 
     * @param field 索引名称，比如"author"、"master"、"tag"等
     * @param fieldValue 索引值，比如author、master、tag等等
     * @param entryId 被索引的条目ID
     */
    @SQL("DELETE FROM ${INDEX_TABLE} WHERE ${INDEX_NAME}=:fieldValue AND entry_id=:entryId")
    void delete(//
            @SQLParam("field") String field, //
            @SQLParam("fieldValue") Object fieldValue, //
            @SQLParam("entryId") long entryId //
    );

    /**
     * 
     * @param field
     * @param fieldValue
     * @param entryIdRange
     * @param lowerSeq
     * @param upperSeq
     * @param size
     * @return
     */
    @SQL("SELECT COUNT(DISTINCT entry_id) FROM ${INDEX_TABLE} WHERE" //
            + " ${INDEX_NAME}=:fieldValue"
            + " #if(:entryId){AND (entry_id BETWEEN :entryId.start AND :entryId.end)}" //
            + " #if(:upperSeq){AND entry_seq < :upperSeq}" //
            + " #if(:lowerSeq){AND entry_seq >= :lowerSeq}")
    int count(//
            @SQLParam("field") String field, //
            @SQLParam("fieldValue") Object fieldValue, //
            @SQLParam("entryId") Range<Long> entryIdRange, // for type query
            @SQLParam("lowerSeq") Long lowerSeq,// 
            @SQLParam("upperSeq") Long upperSeq//
    );

    /**
     * 
     * @param field
     * @param fieldValue
     * @param entryIdRange
     * @param older
     * @param entrySeq
     * @param size
     * @return
     */
    @RowHandler(rowMapper = HitRowMapper.class)
    @SQL("SELECT ${INDEX_SELECT_COLUMNS} FROM ${INDEX_TABLE} WHERE" //
            + " ${INDEX_NAME}=:fieldValue"
            + " #if(:entryId){AND (entry_id BETWEEN :entryId.start AND :entryId.end)}" //
            + " #if(:upperSeq){AND entry_seq < :upperSeq}" //
            + " #if(:lowerSeq){AND entry_seq >= :lowerSeq}" //
            + " ORDER BY entry_seq DESC" //
            + " LIMIT #if(:offset){:offset,} :size")
    List<Entry> find(//
            @SQLParam("field") String field, //
            @SQLParam("fieldValue") Object fieldValue, //
            @SQLParam("entryId") Range<Long> entryIdRange, // for type query
            @SQLParam("lowerSeq") Long lowerSeq,// 
            @SQLParam("upperSeq") Long upperSeq,//
            @SQLParam("offset") Integer offset, //
            @SQLParam("size") int size//
    );

    /**
     * 
     * @param field
     * @param fieldValue
     * @param entries
     * @param size
     * @return
     */
    @RowHandler(rowMapper = HitRowMapper.class)
    @SQL("SELECT ${INDEX_SELECT_COLUMNS} FROM ${INDEX_TABLE} WHERE ${INDEX_NAME}=:fieldValue"
            + " AND entry_id in (:entries) ORDER BY entry_seq DESC LIMIT :size")
    List<Entry> findByIn(//
            @SQLParam("field") String field, //
            @SQLParam("fieldValue") Object fieldValue, //
            @SQLParam("entries") long[] entries,//
            @SQLParam("size") int size//
    );

}

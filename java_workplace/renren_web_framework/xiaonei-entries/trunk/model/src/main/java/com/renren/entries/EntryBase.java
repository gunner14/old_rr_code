package com.renren.entries;

import java.util.List;

import com.renren.entries.event.EntryEventListener;
import com.renren.entries.model.Entry;
import com.renren.entries.model.Field;
import com.renren.entries.model.query.Query;

/**
 * {@link EntryBase}
 * 封装了底层存储的细节以及存储逻辑（包括存储数据结构、数据可扩展性等等的功能或非功能特性），完整提供了对数据的增删改查功能。
 * 
 * @author qieqie.wang@gmail.com
 */
public interface EntryBase {

    /**
     * 增加一个事件侦听器，当记录的创建、更新、删除完成时能够被通知
     * 
     * @param l
     */
    void addEventListener(EntryEventListener l);

    /**
     * @see #addEntry(long, List)
     * @param id
     * @param fields
     * @return
     */
    Entry addEntry(long id, Field... fields);

    /**
     * 新增存储一个记录。
     * <p>
     * 记录的id由调用者提供，fields必须非空而且字段个数不能为0，新增成功后返回对应的 {@link Entry} 对象
     * <p>
     * 
     * 如果底层存储系统配置了按照某个字段来分散，新增记录时侯一定要提供该字段对应的Field
     * 
     * @throws 如果该记录已经存在或者出现了其他的约束错误将抛出相应的异常
     */
    Entry addEntry(long id, List<Field> fields);

    /**
     * @see #updateEntry(long, boolean, List)
     * @param id
     * @param fields
     * @return
     */
    Entry updateEntry(long id, boolean replace, Field... fields);

    /**
     * 对给定id的记录进行部分更新或全部更新。
     * <p>
     * 完全更新是指，将原有的记录数据全部清除并重新保存为fields参数所代表的字段值<br>
     * 部分更新是指，如果传入的fields表示的字段，原记录已经有时更新为最新值，原记录没有则新加进去，原记录的其它字段不受影响
     * <p>
     * 当replace参数为true时表示完全替换更新，这个一定要谨慎使用，大部分情况下不应该使用完全替换更新！ <br>
     * 如果fields中的field的值为null表示要删除原记录中的该字段
     * <p>
     * 如果底层存储系统配置了按照某个字段来分散，fields参数也没有必要一定要带上该字段值，本存储系统能够成功定位并更新对应的记录。
     * 但是如果fields带上了所该分散字段，系统则可省略一次字典查询，相应地也就提高一点点时间
     * 
     */
    Entry updateEntry(long id, boolean replace, List<Field> fields);

    /**
     * 从存储中永久删除该纪录的实体数据以及索引数据
     */
    void deleteEntry(long id);

    /**
     * 返回符合查询条件query的记录个数
     * <p>
     * 此方法中query参数中的字段有所限制，必须符合下面的规则才会被支持，否则会抛出异常：<br>
     * <ul>
     * <li>只能是索引的字段，不能是非索引的字段</li>
     * <li>除规定的索引字段之外，可以再伴随 create_time 和 id 字段（可同时伴随这两个字段）</li>
     * </ul>
     * <p>
     * create_time字段的使用<br>
     * <ul>
     * <li>只能包含一段时间，不能包含多段时间</li>
     * <li>必须提供这段时间的开始时间和结束时间，早的在前晚的在后</li>
     * <li>必须以long的形式提供相应的开始和结束时间</li>
     * </ul>
     * <p>
     * id字段的使用<br>
     * <ul>
     * <li>只能包含一段id区间，不能包含多段区间</li>
     * <li>必须提供这段区间的开始值和结束值，小的在前大的在后</li>
     * <li>必须以long的形式提供相应的开始和结束值</li>
     * </ul>
     * <p>
     * 其它字段的规定<br>
     * <ul>
     * <li>可以包含“or”，比如tag:(123 or 456)</li>
     * <li>相区别的：{@link #find(Query, int, int)}中query的类似字段不能包含多个或只能是tag:123
     * 或者 tag:456，分两次查询</li>
     * </ul>
     * 
     */
    int getCount(Query query);

    /**
     * 返回给定field字段取给定value值的记录列表集合
     * 
     * <p>
     * 本接口支持绝对分页的查询<br>
     * 所谓绝对分页是指，可以对全部符合条件的记录全完全排序，并在每次查询时指定从某个位置开始查询若干个记录<br>
     * 与绝对分页相对应的一个概念是相对分页，即虽然理论上可以对全部符合条件的记录做完全排序，
     * 但每次查询只能根据上一个查询的结果查询下一页或上一页
     * <p>
     * 
     * <p>
     * 此方法中query参数中的字段有所限制，必须符合下面的规则才会被支持，否则会抛出异常：<br>
     * <ul>
     * <li>只能是索引的字段，不能是非索引的字段</li>
     * <li>除规定的索引字段之外，可以再伴随 create_time 和 id 字段（可同时伴随这两个字段）</li>
     * </ul>
     * <p>
     * create_time字段的使用<br>
     * <ul>
     * <li>只能包含一段时间，不能包含多段时间</li>
     * <li>必须提供这段时间的开始时间和结束时间，早的在前晚的在后</li>
     * <li>必须以long的形式提供相应的开始和结束时间</li>
     * </ul>
     * <p>
     * id字段的使用<br>
     * <ul>
     * <li>只能包含一段id区间，不能包含多段区间</li>
     * <li>必须提供这段区间的开始值和结束值，小的在前大的在后</li>
     * <li>必须以long的形式提供相应的开始和结束值</li>
     * </ul>
     * <p>
     * 其它字段的规定<br>
     * <ul>
     * <li>不能包含多个“or”，比如tag:(123 or 456)这是非法的</li>
     * <li>相区别的: {@link #getCount(Query)}中query的类似字段则可以包含多个or</li>
     * </ul>
     * <p>
     * 
     * @param offset 从0开始的正整数, 返回的列表在全部条件的集合中的起始位置
     * @param size 最多返回的个数
     */
    List<Entry> find(Query query, int offset, int size);

    /**
     * 根据给定的query串查询符合条件的记录，并返回
     * <p>
     * 例如: query = "tag:(123 or 456) and create_time:232134-232144"
     * <p>
     * 本方法中query参数中的字段几乎没有过多限制，先后顺序也不做规定，实现者会进行相应的优化。<br>
     * 所包含的字段可以索引的， 亦可是非索引的字段，但至少提供一个索引字段。
     * 
     * <p>
     * 关于分页功能，和 {@link #find(Query, int, int)}
     * 不一样的是，本接口只提供相对分页的方法，不提供绝对分页的支持。<br>
     * 要查询比id为12345678记录更早的记录，请在query中增加older字段，比如：
     * "tag:(123 or 456) and older:12345678"<br>
     * 要查询比id为12345678记录更晚的记录，请在query中增加newer字段，比如：
     * "tag:(123 or 456) and newer:12345678"
     * <p>
     * 
     * @param size 最多返回的个数
     * 
     */
    List<Entry> find(Query query, int size);

    /**
     * 返回给定id的记录，如果找不到返回null
     */
    Entry get(long id);

    /**
     * 批量获取给定id的记录，所返回的数组长度与所传入的id列表长度一致，如果某个id的记录不存在对应的结果数组中的该位置的元素值为null
     */
    Entry[] gets(long[] entries);

    /**
     * 批量获取给定id的记录，所返回的数组长度与所传入的id列表长度一致，如果某个id的记录不存在对应的结果数组中的该位置的元素值为null
     * <p>
     * 
     * fields参数用于告诉本存储系统返回的记录只需要包含给定的这些列
     */
    Entry[] gets(long[] entries, String[] fields);
}

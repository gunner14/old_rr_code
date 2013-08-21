/**
 * 
 */
package com.xiaonei.platform.component.tools;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

/**
 * @author Administrator
 */
public class CollectionUtil {

	@SuppressWarnings("unchecked")
	private static final IterableElementDealer NO_TRANS = new IterableElementDealer<Object, Object>() {
		@Override
		public Object deal(int index, Object from) {
			return from;
		}
	};

	public static void main(String[] args) {
		System.out.println("@@@@@@start");
		List<Integer> list = new ArrayList<Integer>();
		for (int i = 0; i < 100; ++i) {
			list.add(i);
		}

		Subcollection<String> sub1 = getAcceptSubcollectionAndCount(list, null,
				new IterableElementDealer<Integer, String>() {
					@Override
					public String deal(int index, Integer from) {
						return "num " + index + "=" + from;
					}
				}, 1, 2);
		System.out.println(sub1);

		Subcollection<Integer> sub2 = getAcceptSubcollectionAndCount(list, new ElementFilter<Integer>() {
			@Override
			public boolean accept(int index,Integer element) {
				return element % 2 == 0;
			}
		}, 0, 100);
		System.out.println(sub2);

		Collection<Integer> sub3 = getAcceptSubcollection(list, new ElementFilter<Integer>() {
			@Override
			public boolean accept(int index,Integer element) {
				return element % 2 == 0;
			}
		}, 10, 100);
		System.out.println(sub3);
		
		System.out.println("@@@@@@end");
	}

	/**
	 * 从一个类型为F的源集合中根据filter筛选合适的元素,并将其按transer转化成合适的目标元素的集合
	 * @param <F>源集合的元素类型
	 * @param <T>返回集合的元素类型
	 * @param source 源集合
	 * @param filter 源集合元素过滤器.符合过滤器条件的元素才会被处理.如果不需要过滤,可传null
	 * @param transer 对象转换器.
	 * @param offset 从源集合第几个符合条件的元素开始(即"本页"之前的元素们)
	 * @param size 每页大小
	 * @return 一个Subcollection对象.包括"该页"的元素集合,所有满足条件的元素的数量(用于分页),以及本页的开始index,结束的index
	 */
	public static <F, T> Subcollection<T> getAcceptSubcollectionAndCount(Collection<F> source, ElementFilter<F> filter,
			IterableElementDealer<F, T> transer, int offset, int size) {
		if(source == null) {
			return new Subcollection<T>(null);
		}
		if (offset < 0) {
			offset = 0;
		}
		if (size < 0) {
			size = source.size();
		}
		int escaped = 0;
		int allCount = 0;
		Collection<T> c = null;
		Iterator<F> i = source.iterator();
		for (int index = 0; i.hasNext(); index++) {
			F e = i.next();
			if (filter == null || filter.accept(index,e)) {
				if (escaped < offset) {
					escaped++;
				} else if (allCount - escaped < size) {
					if (c == null) {
						c = new ArrayList<T>(size);
					}
					c.add(transer.deal(index, e));
				}
				allCount++;
			}
		}
		if (c == null) {
			c = Collections.emptyList();
		} else {
			c = Collections.unmodifiableCollection(c);
		}
		Subcollection<T> rt = new Subcollection<T>(c);
		rt.setTheNumbers(allCount, offset, size);
		return rt;
	}

	/**
	 * 从一个类型为F的源集合中根据filter筛选合适的元素,并将其按transer转化成合适的目标元素的集合
	 * @param <F>源集合的元素类型
	 * @param <T>返回集合的元素类型
	 * @param source 源集合
	 * @param transer 对象转换器.
	 * @param offset 从源集合第几个符合条件的元素开始(即"本页"之前的元素们)
	 * @param size 每页大小
	 * @return 一个Subcollection对象.包括"该页"的元素集合,所有满足条件的元素的数量(用于分页),以及本页的开始index,结束的index
	 */
	public static <F, T> Subcollection<T> getAcceptSubcollectionAndCount(Collection<F> source,
			IterableElementDealer<F, T> transer, int offset, int size) {
		return getAcceptSubcollectionAndCount(source, null, transer, offset, size);
	}

	/**
	 * 从一个源集合中根据filter筛选合适的元素子集
	 * @param <T>返回集合的元素类型
	 * @param source 源集合
	 * @param filter 源集合元素过滤器.符合过滤器条件的元素才会被处理.如果不需要过滤,可传null
	 * @param offset 从源集合第几个符合条件的元素开始(即"本页"之前的元素们)
	 * @param size 每页大小
	 * @return 一个Subcollection对象.包括"该页"的元素集合,所有满足条件的元素的数量(用于分页),以及本页的开始index,结束的index
	 */
	@SuppressWarnings("unchecked")
	public static <T> Subcollection<T> getAcceptSubcollectionAndCount(Collection<T> source, ElementFilter<T> filter,
			int offset, int size) {
		return getAcceptSubcollectionAndCount(source, filter, NO_TRANS, offset, size);
	}

	@SuppressWarnings("unchecked")
	public static <T> Subcollection<T> getAcceptSubcollectionAndCount(Collection<T> source, int offset, int size) {
		return getAcceptSubcollectionAndCount(source, null, NO_TRANS, offset, size);
	}

	/**
	 * 从一个类型为F的源集合中根据filter筛选合适的元素,并将其按transer转化成合适的目标元素的集合
	 * @param <F>源集合的元素类型
	 * @param <T>返回集合的元素类型
	 * @param source 源集合
	 * @param filter 源集合元素过滤器.符合过滤器条件的元素才会被处理.如果不需要过滤,可传null
	 * @param transer 对象转换器.
	 * @param offset 从源集合第几个符合条件的元素开始(即"本页"之前的元素们)
	 * @param size 每页大小
	 * @return 一个Collection对象(若source为null,则返回Collections.emptyList();若source非null则事实上返回的是ArrayList,可根据需要强制转换).
	 */
	public static <F, T> Collection<T> getAcceptSubcollection(Collection<F> source, ElementFilter<F> filter,
			IterableElementDealer<F, T> transer, int offset, int size) {
		if(source == null) {
			return Collections.emptyList();
		}
		if (offset < 0) {
			offset = 0;
		}
		if (size < 0) {
			size = source.size();
		}
		int escaped = 0;
		int allCount = 0;
		Collection<T> c = null;
		Iterator<F> i = source.iterator();
		for (int index = 0; i.hasNext(); index++) {
			F e = i.next();
			if (filter == null || filter.accept(index,e)) {
				if (escaped < offset) {
					escaped++;
				} else if (allCount - escaped < size) {
					if (c == null) {
						c = new ArrayList<T>(size);
					}
					c.add(transer.deal(index, e));
				} else {
					break;
				}
				allCount++;
			}
		}
		if (c == null) {
			c = Collections.emptyList();
		} else {
			c = Collections.unmodifiableCollection(c);
		}
		return c;
	}

	/**
	 * 从一个类型为F的源集合中根据filter筛选合适的元素,并将其按transer转化成合适的目标元素的集合
	 * @param <F>源集合的元素类型
	 * @param <T>返回集合的元素类型
	 * @param source 源集合
	 * @param transer 对象转换器.
	 * @param offset 从源集合第几个符合条件的元素开始(即"本页"之前的元素们)
	 * @param size 每页大小
	 * @return 一个Collection对象(若source为null,则返回Collections.emptyList();若source非null则事实上返回的是ArrayList,可根据需要强制转换).
	 */
	public static <F, T> Collection<T> getAcceptSubcollection(Collection<F> source,
			IterableElementDealer<F, T> transer, int offset, int size) {
		return getAcceptSubcollection(source, null, transer, offset, size);
	}

	/**
	 * 从一个类型为F的源集合中根据filter筛选合适的元素,并将其按transer转化成合适的目标元素的集合
	 * @param <F>源集合的元素类型
	 * @param <T>返回集合的元素类型
	 * @param source 源集合
	 * @param filter 源集合元素过滤器.符合过滤器条件的元素才会被处理.如果不需要过滤,可传null
	 * @param offset 从源集合第几个符合条件的元素开始(即"本页"之前的元素们)
	 * @param size 每页大小
	 * @return 一个Collection对象(若source为null,则返回Collections.emptyList();若source非null则事实上返回的是ArrayList,可根据需要强制转换).
	 */
	@SuppressWarnings("unchecked")
	public static <T> Collection<T> getAcceptSubcollection(Collection<T> source, ElementFilter<T> filter, int offset,
			int size) {
		return getAcceptSubcollection(source, filter, NO_TRANS, offset, size);
	}

	/**
	 * 从一个类型为F的源集合中根据filter筛选合适的元素,并将其按transer转化成合适的目标元素的集合
	 * @param <F>源集合的元素类型
	 * @param <T>返回集合的元素类型
	 * @param source 源集合
	 * @param offset 从源集合第几个符合条件的元素开始(即"本页"之前的元素们)
	 * @param size 每页大小
	 * @return 一个Collection对象(若source为null,则返回Collections.emptyList();若source非null则事实上返回的是ArrayList,可根据需要强制转换).
	 */
	@SuppressWarnings("unchecked")
	public static <T> Collection<T> getAcceptSubcollection(Collection<T> source, int offset, int size) {
		return getAcceptSubcollection(source, null, NO_TRANS, offset, size);
	}
}

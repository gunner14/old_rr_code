/**
 * 
 */
package com.xiaonei.platform.component.tools;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;

/**
 * @author Administrator
 */
public class Subcollection<T> implements Collection<T>,Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3995895512470225663L;
	
	final private Collection<? extends T> source;
	private int count;
	private int start;
	private int end;
	private int pagesize;

	public Subcollection(Collection<? extends T> collection) {
		super();
		if (collection == null) {
			this.source = Collections.emptyList();
		} else {
			this.source = collection;
		}
	}

	public String toString() {
		return source.toString() + " count=" + count + ",start=" + start + ",end=" + end;
	}

	public int getCount() {
		return count;
	}

	public int getStart() {
		return start;
	}

	public int getEnd() {
		return end;
	}

	/**
	 * 设置总数(count),本页开始数字(start),本页结束数字(end). 使用了本方法就不用setCount,setStart,setEnd了.
	 * 并且会自动判断一些东西,比如end<start,或start>count之类. 注意参数,不是count,start,end,而是count,offset,pagesize
	 * @param _count
	 * @param offset
	 * @param pagesize
	 */
	public void setTheNumbers(Integer _count, Integer offset, Integer pagesize) {
		if (_count == null || _count <= 0) {
			if (source != null) {
				count = source.size();
			} else {
				count = 0;
			}
		} else {
			count = _count;
		}

		if (offset == null || offset <= 0) {
			offset = 0;
		}
		start = offset + 1;
		if (start > count) {
			start = count;
		}

		if (pagesize == null || pagesize <= 0) {
			end = count;
			this.pagesize = 0;
		} else {
			this.pagesize = pagesize;
			end = offset + pagesize;
			if (end > count)
				end = count;
		}
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((source == null) ? 0 : source.hashCode());
		result = prime * result + count;
		result = prime * result + end;
		result = prime * result + start;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Subcollection<?> other = (Subcollection<?>) obj;
		if (source == null) {
			if (other.source != null)
				return false;
		} else if (!source.equals(other.source))
			return false;
		if (count != other.count)
			return false;
		if (end != other.end)
			return false;
		if (start != other.start)
			return false;
		return true;
	}

	/**
	 * @param c
	 * @return
	 * @see java.util.Collection#addAll(java.util.Collection)
	 */
	public boolean addAll(Collection<? extends T> c) {
		throw new UnsupportedOperationException();
	}

	/**
	 * @see java.util.Collection#clear()
	 */
	public void clear() {
		source.clear();
	}

	/**
	 * @param o
	 * @return
	 * @see java.util.Collection#contains(java.lang.Object)
	 */
	public boolean contains(Object o) {
		return source.contains(o);
	}

	/**
	 * @param c
	 * @return
	 * @see java.util.Collection#containsAll(java.util.Collection)
	 */
	public boolean containsAll(Collection<?> c) {
		return source.containsAll(c);
	}

	/**
	 * @return
	 * @see java.util.Collection#isEmpty()
	 */
	public boolean isEmpty() {
		return source.isEmpty();
	}

	/**
	 * @return
	 * @see java.util.Collection#iterator()
	 */
	@SuppressWarnings("unchecked")
	public Iterator<T> iterator() {
		return (Iterator<T>) source.iterator();
	}

	/**
	 * @param o
	 * @return
	 * @see java.util.Collection#remove(java.lang.Object)
	 */
	public boolean remove(Object o) {
		return source.remove(o);
	}

	/**
	 * @param c
	 * @return
	 * @see java.util.Collection#removeAll(java.util.Collection)
	 */
	public boolean removeAll(Collection<?> c) {
		return source.removeAll(c);
	}

	/**
	 * @param c
	 * @return
	 * @see java.util.Collection#retainAll(java.util.Collection)
	 */
	public boolean retainAll(Collection<?> c) {
		return source.retainAll(c);
	}

	/**
	 * @return
	 * @see java.util.Collection#size()
	 */
	public int size() {
		return source.size();
	}

	/**
	 * @return
	 * @see java.util.Collection#toArray()
	 */
	public Object[] toArray() {
		return source.toArray();
	}

	/**
	 * @param <T>
	 * @param a
	 * @return
	 * @see java.util.Collection#toArray(T[])
	 */
	@SuppressWarnings("hiding")
	public <T> T[] toArray(T[] a) {
		return source.toArray(a);
	}

	@Override
	public boolean add(T e) {
		throw new UnsupportedOperationException();
	}

	public int getPagesize() {
		return pagesize;
	}

}

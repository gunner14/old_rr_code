/**
 * 
 */
package com.xiaonei.platform.component.tools;

/**
 * @author Administrator
 *
 */
public interface ElementFilter<E> {

	public boolean accept(int index,E element);
}

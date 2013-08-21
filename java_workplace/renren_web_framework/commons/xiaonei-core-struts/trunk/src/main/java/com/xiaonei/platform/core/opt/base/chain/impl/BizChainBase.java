package com.xiaonei.platform.core.opt.base.chain.impl;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;

import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;

/**
 * 封装BizFilter的chain的实现。它继承自ChainBase，重写了ChainBase的execute方法。
 * 重写的原因是ChainBase的execute方法抛出Exception，而BizChainBase只抛出BizFilterException。
 * 
 * 增加了几个对chain操作的方法，以增加灵活性。
 * 
 * @author Li Weibo
 * @since 2008-12-28 上午12:14:33
 */
public class BizChainBase extends ChainBase {

	/**
	 * 按index删掉一个filter
	 * 
	 * @param index 要删除的filter的index, 0-based.
	 */
	public void deleteFilter(int index) {
		if (index < 0 || index >= commands.length) {
			throw new IndexOutOfBoundsException(index + "");
		}
		Command[] newCommands = new Command[commands.length - 1];
		System.arraycopy(commands, 0, newCommands, 0, index);	//copy前面的
		System.arraycopy(commands, index + 1, newCommands, index, commands.length - index - 1);	//copy后面的
		commands = newCommands;
	}
	
	/**
	 * 按name删除一个filter，如果没有找到指定的filter，会抛出异常.
	 * 
	 * 先按name查找，再按className查找
	 * 
	 * @param filterName 要删除的filter的名字
	 */
	public void deleteFilter(String filterName) {
		int i = 0;
		for (Command c : commands) {
			if (c instanceof AbstractBizFilter) {
				AbstractBizFilter f = (AbstractBizFilter)c;
				if (filterName.equals(f.getName())) {
					deleteFilter(i);
					return;
				}
			} 
			if (c.getClass().getName().equals(filterName)) {
				deleteFilter(i);
				return;
			}
			i ++;
		}
		throw new IllegalArgumentException("No such filter : " + filterName);
	}
	
	/**
	 * 将一个filter插入到chain的指定位置
	 * 
	 * @param filter 待插入的filter
	 * @param index 要插入的位置
	 */
	public void addFilter(int index, BizFilter filter) {
		if (index < 0 || index > commands.length) {
			throw new IndexOutOfBoundsException(index + "");
		}
		Command[] newCommands = new Command[commands.length + 1];	
		System.arraycopy(commands, 0, newCommands, 0, index);	//copy前面的
		newCommands[index] = filter;	//插入
		System.arraycopy(commands, index, newCommands, index + 1, commands.length - index);	//copy后面的
		commands = newCommands;
	}
	
	/**
	 * 按名字获取一个chain中的一个Filter，先按filterName查找，再按className查找
	 * 找不到则返回null
	 * @param filterName 
	 * @return
	 */
	public BizFilter getFilter(String filterName) {
		for (int i = 0; i < commands.length; i++) {
			Command c = commands[i];
			if (c instanceof AbstractBizFilter) {
				AbstractBizFilter f = (AbstractBizFilter)c;
				if (f.getName().equals(filterName)) {
					return f;
				}
			} 
			if (c.getClass().getName().equals(filterName) && c instanceof BizFilter) {
				return (BizFilter)c;
			}
		}
		return null;
	}
	
	@Override
	public boolean execute(Context context) throws BizFilterException {
		boolean ret;
		try {
			ret = super.execute(context);
		} catch (BizFilterException e) {
			throw e;
		} catch (Exception e) {
			e.printStackTrace();
			throw new BizFilterException(BizFilterException.CODE_UNKNOWN_EXCEPTION, e);
		}
		return ret;
	}
	
	private static void testDelete() {
		int[] commands = new int[]{-1,1,2,3,4,5};
		int[] newCommands = new int[commands.length - 1];
		for (int index = 0; index < commands.length; index ++) {
			System.arraycopy(commands, 0, newCommands, 0, index);
			System.arraycopy(commands, index + 1, newCommands, index, commands.length - index - 1);
			for (int i = 0; i < newCommands.length; i++) {
				System.out.print(newCommands[i] + " ");
			}
			System.out.println("");
		}
	}
	
	private static void testAdd() {
		int[] commands = new int[]{-1,1,2,3,4,5};
		int[] newCommands = new int[commands.length + 1];
		int filter = 10;
		for (int index = 0; index <= commands.length; index ++) {
			System.arraycopy(commands, 0, newCommands, 0, index);
			//插入
			newCommands[index] = filter;
			//copy后面的
			System.arraycopy(commands, index, newCommands, index + 1, commands.length - index);
			for (int i = 0; i < newCommands.length; i++) {
				System.out.print(newCommands[i] + " ");
			}
			System.out.println("");
		}
	}
	
	public static void main(String[] args) {
		testDelete();
		testAdd();
	}
}

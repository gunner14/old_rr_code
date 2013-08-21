/*
 * Created on 2003-12-17
 */
package com.xiaonei.platform.core.utility.sched;

/**
 * �����̷߳Ƿ��¼�
 * �����߳��г��ֵ�uncaught��Exception��Ϊ��ֹ���������֣��½��߳�
 * �滻ԭ���̣߳�ԭ�߳�ͣ��ͨ�����֤�̳߳���ʼ���й涨�����߳�����
 * 
 * @author <a href="jing.huang@opi-corp.com">Huang Jing</a>
 */
public interface SchedThreadDeathListener {

	public void notifyThreadEvent(SchedThreadPool pool, Thread t, Throwable e);

}

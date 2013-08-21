/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.xce.footprintunite.VisitFootprintResult;


/**
 * 脚印服务
 * 
 * @author tai.wang@opi-corp.com Sep 8, 2010 - 7:28:02 PM
 */
public interface IVisitFootprintAdapter extends IRegisterable {

	VisitFootprintResult getAll(int owner, int begin, int limit) ;
}

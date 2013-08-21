/**
 *
 */
package com.xiaonei.reg.register.controllers.init;

import com.xiaonei.reg.register.controllers.base.BaseRegInitInviteController;
import com.xiaonei.reg.register.form.RegShortInviteForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;

/**
 * @author tai.wang@opi-corp.com
 */
public class Xn6301Controller extends BaseRegInitInviteController {

	@Override
	public BaseRegForm getRegForm() {
		return new RegShortInviteForm();
	}

}

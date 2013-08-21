package com.xiaonei.reg.register.logic.pretreatment.validate;

import java.text.ParseException;
import java.text.SimpleDateFormat;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IBirthday;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckBirthday extends BaseRegCheckLogic {

	@Override
	public String check(BaseRegForm regForm) {
		IBirthday ib = (IBirthday) regForm;
		if (StringUtils.isEmpty(ib.getBirth_year())
				|| StringUtils.isEmpty(ib.getBirth_month())
				|| StringUtils.isEmpty(ib.getBirth_day())) {
			return ERR_BIRTHDAY_EMPTY;
		}
		String strDate = ib.getBirth_year() + "/" + ib.getBirth_month() + "/"
				+ ib.getBirth_day();
		SimpleDateFormat df = new SimpleDateFormat("yyyy/mm/dd");
		df.setLenient(false);
		try {
			df.parse(strDate);
		} catch (ParseException e) {
			return ERR_BIRTHDAY_ERR;
		}
		return OK;
	}
}

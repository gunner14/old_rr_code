package mop.hi.oce.domain.user;

import xce.tripod.UserConfig.UserConfigData;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.log.Oce;

public class UserConfigInfoFactory {
	public UserConfigInfo create() {
		return new UserConfigInfo();
	}
}
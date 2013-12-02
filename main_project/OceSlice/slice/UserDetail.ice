#ifndef WAP_USER_DETAIL_ICE
#define WAP_USER_DETAIL_ICE

[["java:package:com.xiaonei"]]
module wap {
		module user {
					module detail {
									
									["java:type:java.util.HashMap"] dictionary<string, string> PropertyMap;

									interface UserDetailService {
										void userBornChange(int userId, PropertyMap properties);
										void userNameChange(int userId, PropertyMap properties);
										void userPassportChange(int userId, PropertyMap properties);
										void userUrlChange(int userId, PropertyMap properties);
										void userContactChange(int userId, PropertyMap properties);
									};

					};
		};
};

#endif

namespace cpp com.renren.passport.service
namespace java com.renren.passport.service


struct LoginResponse {
    1: required i32 userId;
    2: required i32 realUserId;
    3: required i32 loginStatusCode;
    4: required string loginStatusdescription;
    5: required string passportTicket;
    6: optional string ticket;
    7: required string account;
    8: required i32 status;
    9: required string domain;
    10:required i32 catchaCount;
    11:required bool weakPassword;
    12:required bool gotoSafeCenter;
    13: required bool sendMail;
    14: optional string oldTicket;
    15: optional string oldPassport;
    
}

struct LoginRequest {
	1: required string userName;
	2: required string md5Password;
	3: required bool autoLogin;
	4: required string origUrl;
	5: required string icode;
	6: required string captchaType;
	7: required string ick;
	8: required i64 expirePeriod;
	9: required i32 loginRegistrationId;
	10: required bool createRenRenTicket;

}

struct LoginContext {
	1: required string host;
	2: required string userAgent;
	3: string referer;
	4: required string ip;
	5: required string requestUrl;
	6: required map<string, string> cooikes;
	7: optional map<string, string> requestContext;
}

struct LogoutResponse {
    1: required i32 logoutStatusCode;
    2: required string logoutStatusdescription;
    
}
enum LoginType {
   Web=0,Wap=1,PlatForm=2,Client=3,MobileClient=4
}
struct TransferRequest {
	1: required string passport;
	2: required string origUrl;
	3: required LoginType loginType;
	4: required i32 loginRegistrationId;
	5: required string userName;
}

struct SwitchRequest {
	1: required string passport;
	2: required string ticket;
	3: required i32 destId;
	4: required i32 realUserId;
	5: required string md5Password;
	6: required i32 loginRegistrationId;
	7: required string icode;
	8: required string captchaType;
	9: required string ick;
	10: required bool createRenRenTicket;
}
struct TicketInfo {
	1: required i32 userId;
	2: required i32 realUserId;
	3: required LoginType loginType;
	4: required string ip;
}
service LoginXoaService {
	LoginResponse login(1:LoginRequest request, 2:LoginContext context);
	LogoutResponse logout(1:string passportTicket, 2:string ticket, 3:LoginContext context);
	LoginResponse transfer(1:TransferRequest request,2:LoginContext context)
	LoginResponse switchAccount(1:SwitchRequest request, 2:LoginContext context);
	i32 getCaptchaGrade(1:string account, 2:string ip);
	i64 verifyPassportForTypes(1:string ticket, 2:list<LoginType> loginTypes);
	i64 verifyPassport(1:string ticket, 2:LoginType loginTypes);
	i64 verifyTicketForTypes(1:string ticket, 2:list<LoginType> loginTypes);
	i64 verifyTicket(1:string ticket, 2:LoginType loginTypes);
	TicketInfo getTicketInfo(1:string ticket);
	TicketInfo getPassportInfo(1:string ticket);
}
service LoginXoaConfigure {
	map<string, list<string>> findLoginConfigure();
}


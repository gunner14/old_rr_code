namespace cpp com.xiaonei.icode.xoa 
namespace java com.xiaonei.icode.xoa

service ValidCodeService {
	bool validCode(1:string inputCode, 2:string type, 3:string id);
}

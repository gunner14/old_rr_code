#ifndef _BEFORE_UPLOADER_H_
#define _BEFORE_UPLOADER_H_

#include "datatype.h"

/**
 * 上传前做的一些事情
 * .如果beforeurl非空,向其传入postdata以外的请求字段
 * .获取返回结果和返回消息
 */
class CBeforeUploader
{
public:	
	bool BeforeUpload(const TRequest& request, TResponse& response);
};
#endif

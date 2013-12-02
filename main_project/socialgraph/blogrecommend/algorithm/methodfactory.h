#ifndef BLOG_METHODFACTORY_H_
#define BLOG_METHODFACTORY_H_

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "base/once.h"
#include "socialgraph/blogrecommend/algorithm/executer.h"
#include "socialgraph/blogrecommend/algorithm/method/blogprocess.h"

namespace xce {
namespace socialgraph {

/*@brief ����blog����ķ���*/
enum Method {
	Participle
};

/*@brief blog���෽���Ĺ�����*/
class MethodFactory {
public:
	static MethodFactory& GetInstance();
	MethodFactory() {}
	virtual ~MethodFactory() {}
	
	/* @brief ��������blogʵ�� */
	BlogProcessPtr Create(Method method);
};

}
}

#endif

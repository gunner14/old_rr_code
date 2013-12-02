/*
 * =====================================================================================
 *
 *       Filename:  indexFactory.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月16日 20时18分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __INDEX_SERVICE_INDEX_FACTORY_H__
#define __INDEX_SERVICE_INDEX_FACTORY_H__

#include "ad/indexService/TIndex/src/targetingIndex.h"
namespace xce{
	namespace ad{

		class IndexFactory
		{
			public:
				static IndexPtr createIndex(const std::string &indexName)
				{
					if(indexName == "TargetingIndex")
					{
						return &(TargetingIndex::instance());
					}
					else
					{
						return NULL;
					}
				}

		};
	}
}
#endif

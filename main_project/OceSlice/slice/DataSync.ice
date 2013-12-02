#ifndef __DATA_SYNC_H__
#define __DATA_SYNC_H__

#include <Util.ice>

module com
{
module renren
{
module datasync
{
	
  class LocalProducerManager
	{
		bool produce(string dbTable, MyUtil::ByteSeq data);
	};

};
};
};

#endif

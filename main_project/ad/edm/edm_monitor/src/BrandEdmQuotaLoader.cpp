#include <string>
#include "../include/BrandEdmQuotaLoader.h"

USE_NS(std)
USE_NS(xercesc)
USE_NS(xce::edm)

DummyRecordPtr BrandEdmQuotaLoader::GetQuota(const EdmInfo& edmInfo){
  Ice::Long id = edmInfo.adGroupID;
  Ice::Long memid = edmInfo.memberID;
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
  	if (adGroups_.find(id) != adGroups_.end()){
      MCE_INFO("BrandEdmQuotaLoader::GetQuota() --> ID: " << id);
      return adGroups_[id];
   	}
  }

	MCE_INFO("BrandEdmQuotaLoader::GetQuota()  New Adgroups groupid:"<<id<<" member:"<<memid);
  AdGroupRecord *adgroupRecord = new AdGroupRecord();
  adgroupRecord->memberId_ = memid;
  {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    adGroups_[id] = DummyRecordPtr(adgroupRecord);
    if(members2Balance_.find(memid) == members2Balance_.end()){
      members2Balance_[memid] = -1;
    }
  }
  return adGroups_[id];
}

void BrandEdmQuotaLoader::UpdateData(){
	vector<string> idVec;
	GetIDs(adGroups_, idVec);
	MCE_INFO("BrandEdmQuotaLoader::UpdataData() adgroupids --> IDs size: " << idVec.size());
	vector<string>::const_iterator vIter = idVec.begin();
	for (; vIter != idVec.end(); ++vIter){
		string url = HTTP + "adgroupids/" + *vIter;
		string result;
		MCE_INFO("BrandEdmQuotaLoader::UpdateData() adgroupids --> Url: " << url);
		GetHttpResult(url, result);
		Parse(result, adGroups_);
  }

	idVec.clear();
	GetIDs2(members2Balance_,idVec);
	MCE_INFO("BrandEdmQuotaLoader::UpdataData() members --> IDs size: " << idVec.size());
	vIter = idVec.begin();
	for (; vIter != idVec.end(); ++vIter){
		string url = HTTP4MEM + "members/" + *vIter;
		string result;
		MCE_INFO("BrandEdmQuotaLoader::UpdateData() --> members Url: " << url);
		GetHttpResult(url, result);
    ParseMembers(result, members2Balance_);
  }
  MCE_DEBUG("BrandEdmQuotaLoader::FillMemberBalance members size:"<<members2Balance_.size());
  FillMemberBalance();
}

void BrandEdmQuotaLoader::Clear(){
	if (adGroups_.size() > LIMIT_SIZE) {
		IceUtil::RWRecMutex::WLock lock(mutex_);
		adGroups_.clear(); 
		members2Balance_.clear();
	}
}

bool BrandEdmQuotaLoader::Parse(const string& result, map<Ice::Long, DummyRecordPtr>& records){
	MCE_INFO("BrandEdmQuotaLoader::Parse --> size: " << result.size() << " content: " << result);
	if (0 == result.size()){
		return false;
	}

	map<Ice::Long, DummyRecordPtr> tempRecords;
	bool ret = false;
	try{
		XMLPlatformUtils::Initialize();
		XercesDOMParser parser;
		char* pointer = new char[result.size() + 1]();
		strcpy(pointer, result.c_str());
		const string bufID = "fake_adgroup_input";
		const XMLByte* p = (const XMLByte*)(pointer);
		MemBufInputSource input(p, strlen(pointer), bufID.c_str(), true);
		parser.parse(input);
		DOMDocument *xmlDoc = parser.getDocument();
		DOMElement* root = xmlDoc->getDocumentElement();
		DOMNodeIterator *iter = xmlDoc->createNodeIterator(root, DOMNodeFilter::SHOW_TEXT, NULL, true);
		Ice::Long key = 0;
		for (DOMNode* current = (DOMNode*)iter->nextNode(); current != 0; current = (DOMNode*)iter->nextNode()){
			ret = GetNodeValue(current, ADGROUP_ID, key);
			if (!ret){
				MCE_WARN("BrandEdmQuotaLoader::Parse --> Parse Key Adgroup ID Error! " << key);
				break;
			}

			AdGroupRecord *adgroupRecord = new AdGroupRecord();
			current = (DOMNode*)iter->nextNode();
			ret = GetNodeValue(current, DAILY_QUOTA, adgroupRecord->dailyQuota_);
			if (!ret){
				MCE_WARN("BrandEdmQuotaLoader::Parse --> Parse Daily Quota Error! " << adgroupRecord->dailyQuota_);
				break;
			}

			current = (DOMNode*)iter->nextNode();
			ret = GetNodeValue(current, DAILY_AMOUNT, adgroupRecord->dailyAmount_);
			if (!ret){
				MCE_WARN("BrandEdmQuotaLoader::Parse --> Parse Daily Amount Error! " << adgroupRecord->dailyAmount_);
				break;
			}

			current = (DOMNode*)iter->nextNode();
			ret = GetNodeValue(current, TOTAL_QUOTA, adgroupRecord->totalQuota_);
			if (!ret){
				MCE_WARN("BrandEdmQuotaLoader::Parse --> Parse Total Quota Error! " << adgroupRecord->totalQuota_);
				break;
			}

			current = (DOMNode*)iter->nextNode();
			ret = GetNodeValue(current, TOTAL_AMOUNT, adgroupRecord->totalAmount_);
			if (!ret){
				MCE_WARN("BrandEdmQuotaLoader::Parse --> Parse Total Amount Error! " << adgroupRecord->totalAmount_);
		  break;
			}

			MCE_INFO("BrandEdmQuotaLoader::Parse --> key: " << key << " total Amount: " << adgroupRecord->totalAmount_ << " total Quota: " << adgroupRecord->totalQuota_ << " daily Amount: " << adgroupRecord->dailyAmount_ << " daily Quota: " << adgroupRecord->dailyQuota_);
			tempRecords[key] = DummyRecordPtr(adgroupRecord);
		}
	} catch (const XMLException& toCatch){
		ret = false;
		MCE_WARN("BrandEdmQuotaLoader::Parse --> XMLException ");
	} catch (const std::exception& e){
		ret = false;
		MCE_WARN("BrandEdmQuotaLoader::Parse --> std::Exception ");
	}
	XMLPlatformUtils::Terminate();

	if (!ret){
		return false;
	}

	{
		IceUtil::RWRecMutex::WLock lock(mutex_);
		map<Ice::Long, DummyRecordPtr>::const_iterator mIter = tempRecords.begin();
		for (; mIter != tempRecords.end(); ++mIter){
			AdGroupRecord * adgroup1= dynamic_cast<AdGroupRecord *>(mIter->second.get());
			AdGroupRecord * adgroup2= dynamic_cast<AdGroupRecord *>(records[mIter->first].get());
			adgroup1->memberId_ = adgroup2->memberId_;
			records[mIter->first] = mIter->second;
		}
	}
	return ret;
}

//填充每个记录的账户余额,便于以后直接使用
void BrandEdmQuotaLoader::FillMemberBalance(){
	MCE_DEBUG("BrandEdmQuotaLoader::FillMemberBalance");
	IceUtil::RWRecMutex::WLock lock(mutex_);
	std::map<Ice::Long, DummyRecordPtr>::iterator ita = adGroups_.begin();
	for(; ita != adGroups_.end(); ++ita){
		AdGroupRecord * adgroup= dynamic_cast<AdGroupRecord *>(ita->second.get());
		std::map<Ice::Long,Ice::Long>::iterator itm = members2Balance_.find(adgroup->memberId_);
		if(itm != members2Balance_.end()){
			adgroup->memberBalance_ = itm->second;
			MCE_DEBUG("BrandEdmQuotaLoader::FillMemberBalance "<<ita->first<<" : "<<adgroup->memberBalance_);
		}
	}
}

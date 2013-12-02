#include <string>
#include "../include/LeftAdEdmQuotaLoader.h"

USE_NS(std)
USE_NS(xercesc)
USE_NS(xce::edm)

DummyRecordPtr LeftAdEdmQuotaLoader::GetQuota(const EdmInfo& edmInfo){
	Ice::Long id = edmInfo.campaignID;
	Ice::Long memid = edmInfo.memberID;
	{
		IceUtil::RWRecMutex::RLock lock(mutex_);
		if (campaigns_.find(id) != campaigns_.end()){
			MCE_INFO("LeftAdEdmQuotaLoader::GetQuota() --> ID: " << id);
			return campaigns_[id];
		}
	}

	MCE_INFO("LeftAdEdmQuotaLoader::GetQuota()  New Adgroups campaignID:"<<id<<" member:"<<memid);
	CampaignRecord * campaignRecord = new CampaignRecord();
	campaignRecord->memberId_ = memid;
	{
		IceUtil::RWRecMutex::WLock lock(mutex_);
		campaigns_[id] = DummyRecordPtr(campaignRecord);
		if(members2Balance_.find(memid) == members2Balance_.end()){
			members2Balance_[memid] = -1;
		}
	}
  return campaigns_[id];
}

void LeftAdEdmQuotaLoader::UpdateData(){
	vector<string> idVec;
	GetIDs(campaigns_, idVec);
	MCE_INFO("SelfQuotaLoader::UpdataData() --> IDs size: " << idVec.size());
	vector<string>::const_iterator vIter = idVec.begin();
	for (; vIter != idVec.end(); ++vIter){
		string url = HTTP + "campaignids/" + *vIter;
		string result;
		MCE_INFO("LeftAdEdmQuotaLoader::UpdateData() --> Url: " << url);
		GetHttpResult(url, result);
		Parse(result, campaigns_);
	}

	idVec.clear();
	GetIDs2(members2Balance_, idVec);
	MCE_INFO("SelfQuotaLoader::UpdataData() members --> IDs size: " << idVec.size());
	vIter = idVec.begin();
	for(; vIter != idVec.end(); ++ vIter){
		string url = HTTP4MEM + "members/" + *vIter;
		string result;
		MCE_INFO("LeftAdEdmQuotaLoader::UpdateData() members --> Url: " << url);
		GetHttpResult(url, result);
		ParseMembers(result, members2Balance_);
	}
	MCE_INFO("LeftAdEdmQuotaLoader::FillMemberBalance members size:"<<members2Balance_.size());
	FillMemberBalance();
}

void LeftAdEdmQuotaLoader::Clear(){
	if (campaigns_.size() > LIMIT_SIZE) {
		IceUtil::RWRecMutex::WLock lock(mutex_);
		campaigns_.clear();
		members2Balance_.clear();
	}
}

bool LeftAdEdmQuotaLoader::Parse(const string& result, map<Ice::Long, DummyRecordPtr>& records){
	map<Ice::Long, DummyRecordPtr> tempRecords;
	if (0 == result.size()){
		return false;
	}

	bool ret = false;
	try{
		XMLPlatformUtils::Initialize();
		XercesDOMParser parser;
		char* pointer = new char[result.size() + 1]();
		strcpy(pointer, result.c_str());
		const string bufID = "fake_campaign_input";
		const XMLByte* p = (const XMLByte*)(pointer);
		MemBufInputSource input(p, strlen(pointer), bufID.c_str(), true);
		parser.parse(input);
		DOMDocument *xmlDoc = parser.getDocument();
		DOMElement* root = xmlDoc->getDocumentElement();
		DOMNodeIterator *iter = xmlDoc->createNodeIterator(root, DOMNodeFilter::SHOW_TEXT, NULL, true);
		Ice::Long key = 0;
		for (DOMNode* current = (DOMNode*)iter->nextNode(); current != 0; current = (DOMNode*)iter->nextNode()){
			CampaignRecord *campaignRecord = new CampaignRecord();
			ret = GetNodeValue(current, CAMPAIGN_ID, key);
			if (!ret){
				MCE_WARN("LeftAdEdmQuotaLoader::Parse --> Parse Campaign ID Key Error! " << key);
				break;
			}

			current = (DOMNode*)iter->nextNode();
			ret = GetNodeValue(current, DAILY_BUDGET, campaignRecord->dailyQuota_);
			MCE_DEBUG("LeftAdEdmQuotaLoader::Parse --> Parse Campaign Daily Quota Value:" << campaignRecord->dailyQuota_);
			if (!ret){
				MCE_WARN("LeftAdEdmQuotaLoader::Parse --> Parse Campaign Daily Quota Value Error! " << campaignRecord->dailyQuota_);
				break;
			}

			current = (DOMNode*)iter->nextNode();
			ret = GetNodeValue(current, DAILY_CONSUME, campaignRecord->dailyAmount_);
			MCE_DEBUG("LeftAdEdmQuotaLoader::Parse --> Parse Campaign Daily Amount Value:" << campaignRecord->dailyAmount_);
			if (!ret){
				MCE_WARN("LeftAdEdmQuotaLoader::Parse --> Parse Campaign Daily Amount Value Error! " << campaignRecord->dailyAmount_);
				break;
			}

			tempRecords[key] = DummyRecordPtr(campaignRecord);
			MCE_INFO("LeftAdEdmQuotaLoader Result: Key: " << key<< " dailyQuota:"<<campaignRecord->dailyQuota_<< " dailyAmount:"<<campaignRecord->dailyAmount_);
		}
	} catch (const XMLException& toCatch){
		ret = false;
		MCE_WARN("LeftAdEdmQuotaLoader::Parse --> XMLException " );
	} catch (const std::exception& e){
		ret = false;
		MCE_WARN("LeftAdEdmQuotaLoader::Parse --> STD Exception ");
	}
	XMLPlatformUtils::Terminate();
	if (!ret){
		MCE_WARN("LeftAdEdmQuotaLoader::Parse Get Quota and amount failed");
		return false;
	}

	{
		IceUtil::RWRecMutex::WLock lock(mutex_);                                   
		map<Ice::Long, DummyRecordPtr>::const_iterator mIter = tempRecords.begin();
		for (; mIter != tempRecords.end(); ++mIter){
			CampaignRecord* campaign1= dynamic_cast<CampaignRecord*>(mIter->second.get());
			MCE_DEBUG("LeftAdEdmQuotaLoader::Parse: quota:"<<campaign1->dailyQuota_ <<" "<<campaign1->dailyAmount_);
			CampaignRecord* campaign2= dynamic_cast<CampaignRecord*>(records[mIter->first].get());
			campaign1->memberId_ = campaign2->memberId_;
			records[mIter->first] = mIter->second;
		}
	}
	return ret;
}

//填充每个记录的账户余额,便于以后直接使用
void LeftAdEdmQuotaLoader::FillMemberBalance(){
	MCE_DEBUG("LeftAdEdmQuotaLoader::FillMemberBalance");
	IceUtil::RWRecMutex::WLock lock(mutex_);
	std::map<Ice::Long, DummyRecordPtr>::iterator ita = campaigns_.begin();
	for(; ita != campaigns_.end(); ++ita){
		CampaignRecord* campaign= dynamic_cast<CampaignRecord*>(ita->second.get());
		std::map<Ice::Long,Ice::Long>::iterator itm = members2Balance_.find(campaign->memberId_);
		if(itm != members2Balance_.end()){
			campaign->memberBalance_ = itm->second;
			MCE_INFO("LeftAdEdmQuotaLoader::FillMemberBalance : "<<ita->first<<" "<<campaign->memberBalance_);
		}
	}
}

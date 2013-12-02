#include <cstdio>
#include <cstring>
#include <sstream>
#include "../include/AdUserFeatureData.h"
#include "../include/AdUserFeatureCacheI.h"

using namespace std;
using namespace MyUtil;
using namespace xce::ad;
using namespace mysqlpp;

const int FILE_NOT_EXIST = -1;
const int FILE_READ_MODE = 2;
const int MD5_LENGTH = 33;
const string TAB = "\t";
const int EQUAL = 0;
const int EXEC_OK = 0;
const string MD5_CMD = "md5sum ";
const string TMP_FILE = "/tmp/md5";
const string READ_MODE = "r";

void AdUserFeatureData::UpdateSQL(const string& sql, const string& reason){
    try{
	const char* DB_HOST = "10.3.23.69";
	const char* DB_NAME = "ad_strategy";
	const char* DB_USER = "ad";
	const char* DB_PASSWORD = "rrad";

        Connection conn(DB_NAME, DB_HOST, DB_USER, DB_PASSWORD);
        Query query(&conn, true, sql.c_str());
	query.store();
    } catch (Ice::Exception& e){
        MCE_WARN("AdUserFeatureData:: " << reason << " user_feature_version ice error! " << e.what());
    } catch (std::exception& e){
        MCE_WARN("AdUserFeatureData:: " << reason << " user_feature_version std error! " << e.what());
    }
}

void AdUserFeatureData::InvalidData(){
    valid_ = false;
    stringstream sql;
    sql << " UPDATE user_feature_version " 
        << " SET valid = 0 "
        << " WHERE local_file = " << localFileName_;
    
    UpdateSQL(sql.str(), "InvalidVersion");
}

string AdUserFeatureData::GetUserFeature(const Ice::Long userID){
    if (userFeatureData_.find(userID) != userFeatureData_.end()){
	MCE_INFO("AdUserFeatureData::GetUserFeature --> Cache UserID: " << userID << " : " << userFeatureData_[userID]);
	return userFeatureData_[userID];
    }
    MCE_INFO("AdUserFeatureData::GetUserFeature --> Not Cache UserID: " << userID);
    return "";
}

bool AdUserFeatureData::IsValidFile(const string& fileName, const string& md5Value){
    if (0 == fileName.size()){
	MCE_WARN("AdUserFeatureData::IsValidFile --> FileName Is Empty!");
	return false;
    }

    if (FILE_NOT_EXIST == access(fileName.c_str(), FILE_READ_MODE)){
	MCE_WARN("AdUserFeatureData::IsValidFile --> Not Exist File: " << fileName);
	return false;
    }

    char md5Char[MD5_LENGTH]; 
    ComputeMd5(fileName, md5Char);
    if ((MD5_LENGTH - 1 != md5Value.size()) || (EQUAL != md5Value.compare(md5Char))){
	MCE_WARN("Md5 From SQL: " << md5Value_ << " Size: " << md5Value.size());
        string value(md5Char);
	MCE_WARN("Md5 From LocalFile: " << value);	
	return false;
    }

    return true;
}

void AdUserFeatureData::ComputeMd5(const string& fileName, char* md5Char){
    string command = MD5_CMD + fileName + " > " + TMP_FILE;
    if (EXEC_OK != system(command.c_str())){
        MCE_WARN("Execute CMD " << command << " Error!" );
        return;
    }
    FILE *fp = fopen(TMP_FILE.c_str(), READ_MODE.c_str());
    if (NULL == fp){
        MCE_WARN("AdUserFeatureData::GetMD5 --> popen file: " << fileName << " failed!");
        md5Char[0] = '\0';
        return;
    }
    else if (NULL == fgets(md5Char, MD5_LENGTH, fp)){
        MCE_WARN("Read MD5 From Stdout Failed!");
        md5Char[0] = '\0';
        return;
    }

    fclose(fp);
}

void AdUserFeatureData::SetMd5Value(const string& md5Value){
    md5Value_ = md5Value;
}

void AdUserFeatureData::SetFileName(const string& fileName){
   localFileName_ = fileName; 
}

string AdUserFeatureData::GetMd5Value(){
    return md5Value_;
}

/*
 * LoadData From File.
 * The Data in File Format: UserID\t\UserFeature\n
 * if Data Load Success, return true; else return false;
 */
bool AdUserFeatureData::LoadData(const string& fileName, const string& md5Value){
    MCE_INFO("AdUserFeatureData:: Begin Load Data: " << fileName);
    fstream in;
    try{
        in.open(fileName.c_str(), fstream::in);
        string line;
        while (getline(in, line).good()){
	    pair<Ice::Long, string> temp = StringSpliter(line, TAB);
            if ((0 == temp.first) || (0 == temp.second.size())){
                MCE_WARN("AdUserFeatureData::LoadData --> Invalid Line: " << line);
	        continue;
            }
	    userFeatureData_.insert(temp);
        }
    } catch (exception& e){
	MCE_WARN("File: " << fileName << " Read Failed!")
        in.close();
	return false;
    }

    in.close();

    localFileName_ = fileName;
    md5Value_ = md5Value;

    MCE_INFO("AdUserFeatureData:: Load Data Finished. Data Size: " << userFeatureData_.size());
    return true;
}

string AdUserFeatureData::ToString(const pair<Ice::Long, string>& data){
   stringstream ss;
   ss << data.first << TAB;
   ss << data.second << endl;
   return ss.str();
}

pair<Ice::Long, string> AdUserFeatureData::StringSpliter(const string& line, const string& tag){
    stringstream ss;
    ss << line;     
    
    pair<Ice::Long, string> data;
    ss >> data.first;
    ss >> data.second;
    return data;
}

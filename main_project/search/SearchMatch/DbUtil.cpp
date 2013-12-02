/*
 * DbUtil.cpp
 *
 *  Created on: 2011-10-27
 *      Author: benjamin
 */

#include "DbUtil.h"
#include <string>

DbUtil::DbUtil()
{
  com::xiaonei::xce::ConnectionPoolManager::instance().initialize();
}

DbUtil::~DbUtil()
{
}

std::vector<Wiki> DbUtil::getWikiListById(int host, int wiki)
{
  com::xiaonei::xce::Statement sql;
  std::string datasource = "social_wiki";
  std::string pattern = "wiki";
  std::vector<Wiki> wiki_list_;
  sql << "SELECT wiki_id, name, state FROM wiki_dictionary WHERE wiki_id > " << wiki << " order by wiki_id asc limit 1000";

  try
  {
    com::xiaonei::xce::QueryRunner(datasource,
        com::xiaonei::xce::CDbRServer,pattern).query(sql, WikiResultHandler(
        wiki_list_));
  } catch (...)
  {
	MCE_INFO("load from mysql error-------- wiki: " + wiki);
  }
  return wiki_list_;
}

bool WikiResultHandler::handle(mysqlpp::Row &row) const
{
  if (!row.empty())
  {
    Wiki wiki;
    wiki.id = (int) row["wiki_id"];
    wiki.name = (std::string) row["name"];
	wiki.state = (int) row["state"];
    
    wiki_list_.push_back(wiki);
  }
  return true;
}

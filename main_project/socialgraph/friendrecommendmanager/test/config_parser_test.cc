#include <iostream>

#include "gtest/gtest.h"

#include "socialgraph/friendrecommendmanager/common/config_parser2.h"

using namespace xce::socialgraph;

class ConfigParserTest : public ::testing::Test {
 public:
	void SetUp() {
		ConfigParser::instance().Initialize("./test2.conf");
	}

	virtual void TearDown() {
	}
};

//-----------------------------------------------------------

TEST_F(ConfigParserTest, GetConfigAsInt) {
	ASSERT_EQ(78, ConfigParser::instance().GetConfigAsInt(ID_O, "v2"));		//int
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsInt(ID_O, "v1"));		//double
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsInt(ID_E2, "nn"));		//string
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsInt(ID_E3, "kseq"));		//seq
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsInt(ID_O, "v4"));		//null
}

TEST_F(ConfigParserTest, GetConfigAsString) {
	ASSERT_EQ("", ConfigParser::instance().GetConfigAsString(ID_O, "v2"));		//int
	ASSERT_EQ("", ConfigParser::instance().GetConfigAsString(ID_O, "v1"));		//double
	ASSERT_EQ("string", ConfigParser::instance().GetConfigAsString(ID_E2, "nn"));		//string
	ASSERT_EQ("", ConfigParser::instance().GetConfigAsString(ID_E3, "kseq"));		//seq
	ASSERT_EQ("", ConfigParser::instance().GetConfigAsString(ID_O, "v4"));		//null
}

TEST_F(ConfigParserTest, GetConfigAsDouble) {
	ASSERT_EQ(0.0, ConfigParser::instance().GetConfigAsDouble(ID_O, "v2"));		//int
	ASSERT_EQ(0.56, ConfigParser::instance().GetConfigAsDouble(ID_O, "v1"));		//double
	ASSERT_EQ(0.0, ConfigParser::instance().GetConfigAsDouble(ID_E2, "nn"));		//string
	ASSERT_EQ(0.0, ConfigParser::instance().GetConfigAsDouble(ID_E3, "kseq"));		//seq
	ASSERT_EQ(0.0, ConfigParser::instance().GetConfigAsDouble(ID_O, "v4"));		//null
}

TEST_F(ConfigParserTest, GetConfigAsStrItemSeq) {
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsStrItemSeq(ID_O, "v2").size());		//int
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsStrItemSeq(ID_O, "v1").size());		//double
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsStrItemSeq(ID_E2, "nn").size());		//string
	ASSERT_EQ(3, ConfigParser::instance().GetConfigAsStrItemSeq(ID_E3, "kseq").size());		//seq
	ASSERT_EQ(0, ConfigParser::instance().GetConfigAsStrItemSeq(ID_O, "v4").size());		//null
}

//-----------------------------------------------------------

TEST_F(ConfigParserTest, GetPriorityAsInt) {
	ASSERT_EQ(7, ConfigParser::instance().GetPriorityAsInt(ID_PRIORITY, "p_1"));			//int
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsInt(ID_PRIORITY, "p_2"));			//double
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsInt(ID_PRIORITY, "p_3"));			//string
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsInt(ID_PRIORITY, "priority"));		//seq	
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsInt(ID_PRIORITY, "v4"));		//null
}

TEST_F(ConfigParserTest, GetPriorityAsString) {
	ASSERT_EQ("", ConfigParser::instance().GetPriorityAsString(ID_PRIORITY, "p_1"));			//int
	ASSERT_EQ("", ConfigParser::instance().GetPriorityAsString(ID_PRIORITY, "p_2"));			//double
	ASSERT_EQ("zn", ConfigParser::instance().GetPriorityAsString(ID_PRIORITY, "p_3"));			//string
	ASSERT_EQ("", ConfigParser::instance().GetPriorityAsString(ID_PRIORITY, "priority"));		//seq	
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsInt(ID_PRIORITY, "v4"));		//null
}

TEST_F(ConfigParserTest, GetPriorityAsDouble) {
	ASSERT_EQ(0.0, ConfigParser::instance().GetPriorityAsDouble(ID_PRIORITY, "p_1"));			//int
	ASSERT_EQ(7.9, ConfigParser::instance().GetPriorityAsDouble(ID_PRIORITY, "p_2"));			//double
	ASSERT_EQ(0.0, ConfigParser::instance().GetPriorityAsDouble(ID_PRIORITY, "p_3"));			//string
	ASSERT_EQ(0.0, ConfigParser::instance().GetPriorityAsDouble(ID_PRIORITY, "priority"));		//seq	
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsInt(ID_PRIORITY, "v4"));		//null
}

TEST_F(ConfigParserTest, GetPriorityAsStrItemSeq) {
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsStrItemSeq(ID_PRIORITY, "p_1").size());			//int
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsStrItemSeq(ID_PRIORITY, "p_2").size());			//double
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsStrItemSeq(ID_PRIORITY, "p_3").size());			//string
	ASSERT_EQ(4, ConfigParser::instance().GetPriorityAsStrItemSeq(ID_PRIORITY, "priority").size());		//seq	
	ASSERT_EQ(0, ConfigParser::instance().GetPriorityAsStrItemSeq(ID_PRIORITY, "v4").size());		//null
}

//-----------------------------------------------------------

TEST_F(ConfigParserTest, GetRuleAsInt) {
	ASSERT_EQ(5, ConfigParser::instance().GetRuleAsInt(ID_RULE, "r_1"));			//int
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsInt(ID_RULE, "r_2"));			//double
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsInt(ID_RULE, "E1"));			//string
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsInt(ID_RULE, "r_3"));		//seq	
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsInt(ID_RULE, "v4"));		//null
}

TEST_F(ConfigParserTest, GetRuleAsString) {
	ASSERT_EQ("", ConfigParser::instance().GetRuleAsString(ID_RULE, "r_1"));			//int
	ASSERT_EQ("", ConfigParser::instance().GetRuleAsString(ID_RULE, "r_2"));			//double
	ASSERT_EQ("random:r_all && whitelist:w_1", ConfigParser::instance().GetRuleAsString(ID_RULE, "E1"));			//string
	ASSERT_EQ("", ConfigParser::instance().GetRuleAsString(ID_RULE, "r_3"));		//seq	
	ASSERT_EQ("", ConfigParser::instance().GetRuleAsString(ID_RULE, "v4"));		//null
}

TEST_F(ConfigParserTest, GetRuleAsDouble) {
	ASSERT_EQ(0.0, ConfigParser::instance().GetRuleAsDouble(ID_RULE, "r_1"));			//int
	ASSERT_EQ(6.4, ConfigParser::instance().GetRuleAsDouble(ID_RULE, "r_2"));			//double
	ASSERT_EQ(0.0, ConfigParser::instance().GetRuleAsDouble(ID_RULE, "E1"));			//string
	ASSERT_EQ(0.0, ConfigParser::instance().GetRuleAsDouble(ID_RULE, "r_3"));		//seq	
	ASSERT_EQ(0.0, ConfigParser::instance().GetRuleAsDouble(ID_RULE, "v4"));		//null
}

TEST_F(ConfigParserTest, GetRuleAsStrItemSeq) {
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsStrItemSeq(ID_RULE, "r_1").size());			//int
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsStrItemSeq(ID_RULE, "r_2").size());			//double
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsStrItemSeq(ID_RULE, "E1").size());			//string
	ASSERT_EQ(4, ConfigParser::instance().GetRuleAsStrItemSeq(ID_RULE, "r_3").size());		//seq	
	ASSERT_EQ(0, ConfigParser::instance().GetRuleAsStrItemSeq(ID_RULE, "v4").size());		//null
}

//-----------------------------------------------------------

TEST_F(ConfigParserTest, GetConstraintAsInt) {
	ASSERT_EQ(3000000, ConfigParser::instance().GetConstraintAsInt(ID_CONSTRAINT, "whitelist"));			//int
	ASSERT_EQ(0, ConfigParser::instance().GetConstraintAsInt(ID_CONSTRAINT, "random"));			//double
	ASSERT_EQ(0, ConfigParser::instance().GetConstraintAsInt(ID_CONSTRAINT, "c_1"));			//string
	ASSERT_EQ(0, ConfigParser::instance().GetConstraintAsInt(ID_CONSTRAINT, "c_2"));		//seq	
}

TEST_F(ConfigParserTest, GetConstraintAsString) {
	ASSERT_EQ("", ConfigParser::instance().GetConstraintAsString(ID_CONSTRAINT, "whitelist"));			//int
	ASSERT_EQ("", ConfigParser::instance().GetConstraintAsString(ID_CONSTRAINT, "random"));			//double
	ASSERT_EQ("string", ConfigParser::instance().GetConstraintAsString(ID_CONSTRAINT, "c_1"));			//string
	ASSERT_EQ("", ConfigParser::instance().GetConstraintAsString(ID_CONSTRAINT, "c_2"));		//seq	
}

TEST_F(ConfigParserTest, GetConstraintAsDouble) {
	ASSERT_EQ(0.0, ConfigParser::instance().GetConstraintAsDouble(ID_CONSTRAINT, "whitelist"));			//int
	ASSERT_EQ(0.4, ConfigParser::instance().GetConstraintAsDouble(ID_CONSTRAINT, "random"));			//double
	ASSERT_EQ(0.0, ConfigParser::instance().GetConstraintAsDouble(ID_CONSTRAINT, "c_1"));			//string
	ASSERT_EQ(0.0, ConfigParser::instance().GetConstraintAsDouble(ID_CONSTRAINT, "c_2"));		//seq	
}

TEST_F(ConfigParserTest, GetConstraintAsStrItemSeq) {
	ASSERT_EQ(0, ConfigParser::instance().GetConstraintAsStrItemSeq(ID_CONSTRAINT, "whitelist").size());			//int
	ASSERT_EQ(0, ConfigParser::instance().GetConstraintAsStrItemSeq(ID_CONSTRAINT, "random").size());			//double
	ASSERT_EQ(0, ConfigParser::instance().GetConstraintAsStrItemSeq(ID_CONSTRAINT, "c_1").size());			//string
	ASSERT_EQ(4, ConfigParser::instance().GetConstraintAsStrItemSeq(ID_CONSTRAINT, "c_2").size());		//seq	
}

//-----------------------------------------------------------

TEST_F(ConfigParserTest, GetSelectorProperty) {
	Json::Value v = ConfigParser::instance().GetSelectorProperty(ID_RANDOM, R_ALL);
	ASSERT_EQ(0.7, v["O"].asDouble());

	Json::Value v1 = ConfigParser::instance().GetSelectorProperty(ID_WHITELIST, W_1);
	EXPECT_FALSE(v1.isNull());

	Json::Value v2 = ConfigParser::instance().GetSelectorProperty(ID_WHITELIST, "null");
	EXPECT_TRUE(v2.isNull());
}

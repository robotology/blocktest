 
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "test.h"
#include "testsDepot.h"
#include "actionNop.h"


using namespace BlockTestCore;
using namespace GenericActions;
using namespace pugi;
using namespace testing;
using ::testing::Matcher;
using ::testing::_;
using ::testing::InvokeArgument;


class ActionNop_Mock: public ActionNop  {
  public:
  MOCK_METHOD(void, getCommandAttribute,(const std::string& ,std::string& ),(const,override));
  MOCK_METHOD(void, getCommandAttribute,(const std::string& ,bool& ),(const,override));

  ActionNop_Mock(const CommandAttributes& commandAttributes,const std::string& testCode):ActionNop(commandAttributes,testCode)
  {}
};

TEST(Test, Test_003) {

  ActionNop_Mock actionNop(CommandAttributes(),"001");

  std::string tmp{""};
  
  InSequence s;
  EXPECT_CALL(actionNop, getCommandAttribute("fixvalue",Matcher<std::string&>(_))).Times(1);
  EXPECT_CALL(actionNop, getCommandAttribute("tables",Matcher<std::string&>(_))).Times(1);
  EXPECT_CALL(actionNop, getCommandAttribute("printerror",Matcher<bool&>(_))).Times(1);
  EXPECT_CALL(actionNop, getCommandAttribute("justonetime",Matcher<bool&>(_))).Times(1);
  EXPECT_CALL(actionNop, getCommandAttribute("printtestinfo",Matcher<bool&>(_))).Times(1);
  actionNop.beforeExecute();
}

TEST(Test, Test_004) {

  ActionNop_Mock actionNop(CommandAttributes(),"001");
 
  execution res=actionNop.execute({1,1});
  EXPECT_EQ(res,BlockTestCore::execution::continueexecution);
}
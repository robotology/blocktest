 
#include "gtest/gtest.h"
#include "tableNormal.h"
#include "tableIncrement.h"


using namespace BlockTestCore;

TEST(Table, TableNormal_001) {
    
    TableNormal tb;
    tb.Init({"testtable","normal","1","1-data","2-data","3-data"});

    EXPECT_EQ(tb.get(),"1-data");
    EXPECT_EQ(tb.get(),"2-data");
    EXPECT_EQ(tb.get(),"3-data");
    EXPECT_EQ(tb.get(),"1-data");
    EXPECT_EQ(tb.get(),"2-data");
    EXPECT_EQ(tb.get(),"3-data");
}

TEST(Table, TableNormal_002) {
    
    TableNormal tb;
    tb.Init({"testtable","normal","1","1-data ext","2-data ext","3-data ext"});

    EXPECT_EQ(tb.get(),"1-data ext");
    EXPECT_EQ(tb.get(),"2-data ext");
    EXPECT_EQ(tb.get(),"3-data ext");
    EXPECT_EQ(tb.get(),"1-data ext");
    EXPECT_EQ(tb.get(),"2-data ext");
    EXPECT_EQ(tb.get(),"3-data ext");
}

TEST(Table, TableNormal_003) {
    
    TableNormal tb;
    tb.Init({"testtable","normal","2","1-data","2-data","3-data","4-data"});

    EXPECT_EQ(tb.get(),"1-data");
    EXPECT_EQ(tb.get(),"1-data");
    EXPECT_EQ(tb.get(),"2-data");
    EXPECT_EQ(tb.get(),"2-data");
    EXPECT_EQ(tb.get(),"3-data");
    EXPECT_EQ(tb.get(),"3-data");    
    EXPECT_EQ(tb.get(),"4-data");
    EXPECT_EQ(tb.get(),"4-data");        
    EXPECT_EQ(tb.get(),"1-data");
    EXPECT_EQ(tb.get(),"1-data");
    EXPECT_EQ(tb.get(),"2-data");
    EXPECT_EQ(tb.get(),"2-data");
    EXPECT_EQ(tb.get(),"3-data");
    EXPECT_EQ(tb.get(),"3-data");    
    EXPECT_EQ(tb.get(),"4-data");
    EXPECT_EQ(tb.get(),"4-data");            
}

TEST(Table, TableNormal_004) {
    
    TableNormal tb;
    tb.Init({"testtable","normal","error","1-data","2-data","3-data"});

    EXPECT_EQ(tb.get(),"1-data");
}

TEST(Table, TableIncrement_001) {
    
    TableIncrement tb;
    tb.Init({"testtable","increment","1","5","3","20"});

    EXPECT_EQ(tb.get(),"3");
    EXPECT_EQ(tb.get(),"8");
    EXPECT_EQ(tb.get(),"13");
    EXPECT_EQ(tb.get(),"18");
    EXPECT_EQ(tb.get(),"3");
    EXPECT_EQ(tb.get(),"8");
    EXPECT_EQ(tb.get(),"13");
    EXPECT_EQ(tb.get(),"18");
}

TEST(Table, TableIncrement_002) {
    
    TableIncrement tb;
    tb.Init({"testtable","increment","1","0.2","3","4"});

    EXPECT_EQ(tb.get(),"3");
    EXPECT_EQ(tb.get(),"3.2");
    EXPECT_EQ(tb.get(),"3.4");
    EXPECT_EQ(tb.get(),"3.6");
    EXPECT_EQ(tb.get(),"3.8");
    EXPECT_EQ(tb.get(),"4");
    EXPECT_EQ(tb.get(),"3");
    EXPECT_EQ(tb.get(),"3.2");
}

TEST(Table, TableIncrement_004) {
    
    TableIncrement tb;
    tb.Init({"testtable","increment","1","-5","20","3"});

    EXPECT_EQ(tb.get(),"20");
    EXPECT_EQ(tb.get(),"15");
    EXPECT_EQ(tb.get(),"10");
    EXPECT_EQ(tb.get(),"5");
    EXPECT_EQ(tb.get(),"20");
    EXPECT_EQ(tb.get(),"15");
    EXPECT_EQ(tb.get(),"10");
    EXPECT_EQ(tb.get(),"5");
}

TEST(Table, TableIncrement_005) {
    
    TableIncrement tb;
    tb.Init({"testtable","increment","1","-0.2","4","3"});

    EXPECT_EQ(tb.get(),"4");
    EXPECT_EQ(tb.get(),"3.8");
    EXPECT_EQ(tb.get(),"3.6");
    EXPECT_EQ(tb.get(),"3.4");
    EXPECT_EQ(tb.get(),"3.2");
    EXPECT_EQ(tb.get(),"4");
    EXPECT_EQ(tb.get(),"3.8");
    EXPECT_EQ(tb.get(),"3.6");
}

TEST(Table, TableIncrement_006) {
    
    TableIncrement tb;
    tb.Init({"testtable","increment","1","-5","-3","-20"});

    EXPECT_EQ(tb.get(),"-3");
    EXPECT_EQ(tb.get(),"-8");
    EXPECT_EQ(tb.get(),"-13");
    EXPECT_EQ(tb.get(),"-18");
    EXPECT_EQ(tb.get(),"-3");
    EXPECT_EQ(tb.get(),"-8");
    EXPECT_EQ(tb.get(),"-13");
    EXPECT_EQ(tb.get(),"-18");
}
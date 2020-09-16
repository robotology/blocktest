 
#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "Table.TableNormal_001";
    //::testing::GTEST_FLAG(filter) = "Table.TableIncrement_002";

    return RUN_ALL_TESTS();
}
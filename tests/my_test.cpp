#include <gtest/gtest.h>

namespace {
    int getMeaningOfLife() {
        return 42;
    }
}

TEST(TestTopic, TrivialEquality) {
    EXPECT_EQ(42, getMeaningOfLife());
}

TEST(TestTopic, MoreEqualityTests) {
    ASSERT_EQ(8, getMeaningOfLife()) << "On ho, a table is broken!";
    EXPECT_FLOAT_EQ(23.23F, 23.23F);
}

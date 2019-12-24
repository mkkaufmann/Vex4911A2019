#include "../include/util/util.hpp"
#include "../include/util/pointutil.hpp"
#include "../include/util/point.hpp"
#include <gtest/gtest.h>

TEST(DistanceTest, PositiveNos){
		Point a = Point(10,10);
		Point b = Point(0,10);
		ASSERT_EQ(PointUtil::distance(a,b), 10);
}

int main(int argc, char **argv){
		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
}

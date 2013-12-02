#include "hello_world.h"
#include <gtest/gtest.h>

TEST(HelloWorld, Constructor) {
    std::string hello = "hello";
    HelloWorld hello_world(hello);
    ASSERT_EQ(hello, hello_world.SayYaa());
}

TEST(HelloWorld, MemLeak) {
    std::string hello = "hello";
    HelloWorld *hello_world = new HelloWorld(hello);
    ASSERT_EQ(hello, hello_world->SayYaa());
}


int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

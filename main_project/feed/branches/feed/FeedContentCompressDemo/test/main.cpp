/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  demo for compress.
 *
 *        Version:  1.0
 *        Created:  2012年02月21日 10时48分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#include "feed/FeedMemcProxy/client/compress_wrap.h"
#include "FeedContentCompressDemoAdapter.h"

using namespace MyUtil;
using namespace xce::feed;

int main() {
	long feedid_begin = 15350511817;
	for (long feedid = feedid_begin;feedid < feedid_begin + 60000000; feedid++) {
		try {
			//	std::string source = "永远不要去想念你不曾拥有的，如果不去拥有，就不会想念。 2、我很挑剔，我很难搞，我讲话难听，我常常脸臭，可是那就是我啊，我就是没有办法像你一样听话顺从又牺牲奉献，就因为我没有这么伟大，我就不配爱人，我就不值得被爱吗？\t\n瞬间戳到...";
			//	std::cout << "source: " << source << " sizeof(source): " << source.length() << std::endl; 
			/*
			   std::string * result = new string;

			   std::cout << "source: " << source << " sizeof(source): " << source.length() << std::endl; 
			   bool isCompressed = CompressWrap::instance().Compress(source, result);

			   if (isCompressed) {
			   */
			//			std::cout << "result: " << result << " sizeof(result): " << result->length() << std::endl; 

			//			std::cout << "Decompress!!" << std::endl;

			//		std::string * de_result = new string;
			//		CompressWrap::instance().Decompress(source.c_str(), de_result);
			//		std::cout << "de_result: " << *(de_result) << " sizeof(de_result): " << de_result->length() <<std::endl;
			//		delete de_result;
			//		}

			//	delete result;

			FeedContentCompressDemoAdapter::instance().compressAndDecompress(feedid);

			if (feedid%3000 == 0) sleep(2); 

		} catch (...) {
			std::cout << "Exception!" << std::endl;
		}
	}
	std::cout << "success!" << std::endl;
	exit(0);
}

/*
 * =====================================================================================
 *
 *       Filename:  util.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月10日 15时27分31秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <sys/time.h>
#include <boost/lexical_cast.hpp>

#ifndef __INDEX_SERVICE_UTIL_H__
#define __INDEX_SERVICE_UTIL_H__

namespace xce{
	namespace ad{

#define CAST_NO_EXCEPTIONS(P, T, V, dft)  					\
		try                                     			\
		{                                       			\
			V = boost::lexical_cast<T>(P);					\
		}                                       			\
		catch(...)                        					\
		{                                       			\
			V = dft;									\
		}																			


		void INT_CAST(const std::string &key, int &val, int dft){

			CAST_NO_EXCEPTIONS(key, int, val, dft);
		}

		void UNSIGNED_CAST(const std::string &key, unsigned &val, unsigned dft){

			CAST_NO_EXCEPTIONS(key, unsigned, val, dft);
		}

		void UINT64_CAST(const std::string &key, uint64_t &val, uint64_t dft){

			CAST_NO_EXCEPTIONS(key, uint64_t, val, dft);
		}

		void BOOL_CAST(const std::string &key, bool &val, bool dft){

			CAST_NO_EXCEPTIONS(key, bool, val, dft);
		}

		void DOUBLE_CAST(const std::string &key, double &val, double dft){

			CAST_NO_EXCEPTIONS(key, double, val, dft);
		}

		uint64_t getMsec(){

			struct timeval tv;
			gettimeofday(&tv, NULL);

			return tv.tv_sec * 1000 + tv.tv_usec / 1000;
		}

	}
}

#endif

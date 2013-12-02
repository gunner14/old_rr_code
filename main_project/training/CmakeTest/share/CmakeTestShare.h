#ifndef __TRAINNING_CMAKETESTSHARE_H__
#define __TRAINNING_CMAKETESTSHARE_H__

/*
 * 1. Cmake编译规则学习
 * 2. 静态工具类学习
 */
namespace training {
	class CmakeTestShare {
	public:
		int get();
		void set(int a);
	private:	
		int _a;
	};

	class StaticShare {
		public:
			static bool init();
			static int getB();
		private:
			static bool _init;
			static int _b;
	};
}
#endif

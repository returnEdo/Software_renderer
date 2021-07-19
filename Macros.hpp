#include <iostream>


#define PRINT(tVal) std::cout << (tVal) << std::endl

#define PRINT_AUTO(tVal) std::cout << "\t" << #tVal << ":\t" << (tVal) << std::endl;

#define PRINT_ARRAY(tArray) \
	for (auto tVal: tArray)\
	{\
		PRINT(tVal);\
	}\

#define R_ASSERT(tCondition, tMessage) \
        if (not (tCondition)) \
        { \
                std::cout << "\n\t- - - - - - - - - - - - - - - - - - - - - - " << std::endl; \
                std::cout << "\n\tRASTERIZER ASSERTION failed ..." << std::endl; \
                std::cout << "\tin FILE:\t" << __FILE__ << std::endl; \
                std::cout << "\tin FUNCTION:\t" << __func__ << std::endl; \
                std::cout << "\tat LINE:\t" << __LINE__ << std::endl; \
                std::cout << "\tRASTERIZER ASSERTION message: " << std::endl; \
                std::cout << "\t" << tMessage << std::endl; \
                std::cout << "\n\t- - - - - - - - - - - - - - - - - - - - - - " << std::endl; \
                exit(1); \
        }

#define LOOP(tIndex, tLow, tTop, tIncrement) \
	for (int tIndex = tLow; tIndex < tTop; tIndex += tIncrement)

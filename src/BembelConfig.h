#ifndef BEMBEL_CONFIG_H
#define BEMBEL_CONFIG_H

// Windows DLL build
#ifdef WIN32
	#ifdef BEMBEL_STATIC
		#define BEMBEL_API
	#else
		#if BEMBEL_EXPORT
			#define BEMBEL_API __declspec(dllexport)
		#else
			#define BEMBEL_API __declspec(dllimport)
		#endif
	#endif
#else // no Windows or static build
	#define BEMBEL_API
#endif

#endif //include guards

#pragma once


#include <cassert>
#include <memory>


template<typename Context>
class GameContext final
{
	private:
		static Context* s_context;


	public:
		static void Register(const std::unique_ptr<Context>& context)
		{
			s_context = context.get();
		}


		static Context* Get()
		{
			assert(s_context && "[GameContext] The context has not been registered.");

			return s_context;
		}
};



template<typename Context>
Context* GameContext<Context>::s_context = nullptr;
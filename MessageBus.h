#include <string>
#include <functional>
#include <map>
#include "Any.h"
#include "function_traits.h"
#include "NonCopyable.h"

using namespace std;

class MessageBus : NonCopyable
{
	public:
		//register Message
		template<typename F>
		void Attach(F && f, const string& strTopic="")
		{
			auto func = to_function(std::forward<F>(f));
			Add(strTopic, std::move(func));
		}

		//send message
		template<typename R>
		void SendReq(const string &strTopic="")
		{
			using function_type = std::function<R()>;
			string strMsgType = strTopic + typeid(function_type).name();
			auto range = m_map.equal_range(strMsgType);  //range  std::pair<iterator, iterator>
			//std::pair<std::multimap<string, Any>::iterator, std::multimap<string, Any>::iterator> range = m_map.equal_range(strMsgType);
			for(Iterater it=range.first; it!=range.second; ++it)
			{
				auto f = it->second.AnyCast<function_type>();
				f();
			}
		}

		template<typename R, typename... Args>
		void SendReq(Args&&... args, const string& strTopic="")
		{
			using function_type = std::function<R(Args...)>;
			string strMsgType = strTopic + typeid(function_type).name();
			auto range = m_map.equal_range(strMsgType);
			for(Iterater it=range.first; it!=range.second; ++it)
			{
				auto f = it->second.AnyCast<function_type>();
				f(std::forward<Args>(args)...);
			}
		}

		template<typename R, typename... Args>
		void Remove(const string& strTopic="")
		{
			using function_type = std::function<R(Args...)>;

			string strMsgType = strTopic + typeid(function_type).name();
			int count = m_map.count(strMsgType);
			auto range = m_map.equal_range(strMsgType);
			m_map.erase(range.first, range.second);
		}

	private:
		template<typename F>
		void Add(const string& strTopic, F && f)
		{
			string strMsgType = strTopic + typeid(F).name();
			m_map.emplace(std::move(strMsgType), std::forward<F>(f));
		}

	private:
		std::multimap<string, Any> m_map;
		typedef std::multimap<std::string, Any>::iterator Iterater;

};

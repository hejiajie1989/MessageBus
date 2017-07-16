/*
 * THIS CLASS CAN DEFINE A CALSS WHICH NOT SUPPORT COPY CONSTRUTOR
 * AND COPY OPERATOR FUNCTION
 */

class NonCopyable
{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
};

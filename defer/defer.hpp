#include <exception>

namespace detail {

#define CONCATSTRIMPL(s1, s2) s1##s2
#define CONCATSTR(s1, s2) CONCATSTRIMPL(s1, s2)

#ifdef __COUNTER__
#define ALLOCATEOBJ(what) auto CONCATSTR(what, __COUNTER__)
#else
#define ALLOCATEOBJ(what) auto CONCATSTR(what, __LINE__)
#endif

	template <typename FunctionType> class ScopeGuardExit { // For all exits.
		FunctionType fn_;
		UncaughtExceptionCounter ec_;

	  public:
		explicit ScopeGuardExit(const FunctionType& fn) : fn_(fn) {}
		explicit ScopeGuardExit(FunctionType&& fn) : fn_(std::move(fn)) {}
		~ScopeGuardExit() noexcept { fn_(); }
	};

	enum class ScopeGuardOnExit {};

	template <typename FunctionType>
	ScopeGuardExit<typename std::decay<FunctionType>::type>
		operator+(::detail::ScopeGuardOnExit, FunctionType&& fn) {
		return ScopeGuardExit<typename std::decay<FunctionType>::type>(
			std::forward<FunctionType>(fn));
	}
}

#define defer                                                                  \
	ALLOCATEOBJ(defer_) = ::detail::ScopeGuardOnExit() + [&]() noexcept

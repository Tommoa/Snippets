#include <exception>

#define CONCATSTRIMPL(s1, s2) s1##s2
#define CONCATSTR(s1, s2) CONCATSTRIMPL(s1, s2)

#ifdef __COUNTER__
#define ALLOCATEOBJ(what) auto CONCATSTR(what, __COUNTER__)
#else
#define ALLOCATEOBJ(what) auto CONCATSTR(what, __LINE__)
#endif

namespace detail {

	struct UncaughtExceptionCounter {
		int ex_ = 0;
		UncaughtExceptionCounter() : ex_(std::uncaught_exceptions()) {}
		bool NewUncaughtException() { return std::uncaught_exceptions() > ex_; }
	};

	template <typename FunctionType, bool executeOnException>
	class ScopeGuard { // For Success and Failure
		FunctionType fn_;
		UncaughtExceptionCounter ec_;

	  public:
		explicit ScopeGuard(const FunctionType& fn) // reference
			: fn_(fn) {}
		explicit ScopeGuard(FunctionType&& fn) // rvalue reference
			: fn_(std::move(fn)) {}
		~ScopeGuard() noexcept(executeOnException) {
			if (executeOnException == ec_.NewUncaughtException()) {
				fn_();
			}
		}
	};

	template <typename FunctionType> class ScopeGuardExit { // For all exits.
		FunctionType fn_;
		UncaughtExceptionCounter ec_;

	  public:
		explicit ScopeGuardExit(const FunctionType& fn) : fn_(fn) {}
		explicit ScopeGuardExit(FunctionType&& fn) : fn_(std::move(fn)) {}
		~ScopeGuardExit() noexcept { fn_(); }
	};

	enum class ScopeGuardOnFail {};
	enum class ScopeGuardOnSuccess {};
	enum class ScopeGuardOnExit {};

	template <typename FunctionType>
	ScopeGuard<typename std::decay<FunctionType>::type, true>
		operator+(::detail::ScopeGuardOnFail, FunctionType&& fn) {
		return ScopeGuard<typename std::decay<FunctionType>::type, true>(
			std::forward<FunctionType>(fn));
	}

	template <typename FunctionType>
	ScopeGuard<typename std::decay<FunctionType>::type, false>
		operator+(::detail::ScopeGuardOnSuccess, FunctionType&& fn) {
		return ScopeGuard<typename std::decay<FunctionType>::type, false>(
			std::forward<FunctionType>(fn));
	}

	template <typename FunctionType>
	ScopeGuardExit<typename std::decay<FunctionType>::type>
		operator+(::detail::ScopeGuardOnExit, FunctionType&& fn) {
		return ScopeGuardExit<typename std::decay<FunctionType>::type>(
			std::forward<FunctionType>(fn));
	}
}

#define defer_fail                                                             \
	ALLOCATEOBJ(defer_fail_) = ::detail::ScopeGuardOnFail() + [&]() noexcept
#define defer_success                                                          \
	ALLOCATEOBJ(defer_success_) = ::detail::ScopeGuardOnSuccess() + [&]()
#define defer                                                                  \
	ALLOCATEOBJ(defer_) = ::detail::ScopeGuardOnExit() + [&]() noexcept

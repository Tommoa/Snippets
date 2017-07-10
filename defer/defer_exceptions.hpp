#ifndef __SNIPPETS_DEFER_EXCEPTIONS_HPP__
#define __SNIPPETS_DEFER_EXCEPTIONS_HPP__
#include "defer.hpp"
#include <exception>

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

	enum class ScopeGuardOnFail {};
	enum class ScopeGuardOnSuccess {};

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

#define defer_fail                                                             \
	ALLOCATEOBJ(defer_fail_) = ::detail::ScopeGuardOnFail() + [&]() noexcept
#define defer_success                                                          \
	ALLOCATEOBJ(defer_success_) = ::detail::ScopeGuardOnSuccess() + [&]()
}
#endif

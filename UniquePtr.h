
#include <type_traits>
#include <utility>
template<class _Tp>
struct DefaultDeleter {
	void operator()(_Tp *_M_p)const {
		delete _M_p;
	}
};
template<class _Tp,class _Deleter= DefaultDeleter<_Tp>>
struct UniquePtr {
	_Tp * _M_p;
	_Deleter _M_deleter;

	explicit UniquePtr(_Tp *p) noexcept : _M_p(p) {} // 自定义构造函数


	template <class _Up, class _UDeleter, class = std::enable_if_t<std::is_convertible_v<_Up *, _Tp *>>> // 没有 C++20 的写法
	// template <class _Up, class _UDeleter> requires (std::convertible_to<_Up *, _Tp *>) // 有 C++20 的写法
	UniquePtr(UniquePtr<_Up, _UDeleter> &&__that) noexcept : _M_p(__that._M_p) {  // 从子类型_Up的智能指针转换到_Tp类型的智能指针
		__that._M_p = nullptr;
	}


	UniquePtr(UniquePtr const &__that) = delete; // 拷贝构造函数

	UniquePtr &operator=(UniquePtr const &__that) = delete; // 拷贝赋值函数

	UniquePtr(UniquePtr &&__that) noexcept : _M_p(__that._M_p) { // 移动构造函数
		__that._M_p = nullptr;
	}

	UniquePtr &operator=(UniquePtr &&__that) noexcept { // 移动赋值函数
		if (this != &__that)[[likely]]{
			if (_M_p)
				_M_deleter(_M_p);
			_M_p = exchange(__that._M_p, nullptr);
		}
		return *this;
	}
	~UniquePtr() {
		if (_M_p != nullptr) {
			_M_deleter(_M_p);
		}
	}
	_Tp* get() {
		return _M_p;
	}

	_Tp *operator->() const noexcept {
		return _M_p;
	}
	_Tp *release() noexcept {
		_Tp *__p = _M_p;
		_M_p = nullptr;
		return __p;
	}
	bool operator==(UniquePtr<_Tp>_that_p) {
		return _that_p == _M_p;
	}
	bool operator!=(UniquePtr<_Tp>_that_p) {
		return _that_p == _M_p;
	}
};
template <class _Tp, class ..._Args>
UniquePtr<_Tp> makeUnique(_Args &&...__args) {
	return UniquePtr<_Tp>(new _Tp(std::forward<_Args>(__args)...));
}


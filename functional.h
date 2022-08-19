#pragma once

namespace tinySTL
{
template <typename T>
class function;

template <typename Ret, typename... Args>
class function<Ret(Args...)>
{
    struct callable_base {
        virtual Ret operator()(Args &&...a) = 0;  // line 1 **
        virtual struct callable_base *copy() const = 0;
        virtual ~callable_base(){};
    };

    template <typename T>
    struct callable_derived : public callable_base {
        T f;
        callable_derived(T functor) : f(functor) {}
        Ret operator()(Args &&...a) { return f(std::forward<Args>(a)...); }
        struct callable_base *copy() const
        {
            return new callable_derived<T>(f);
        }
    };

  public:
    template <typename T>
    function(T functor) : base(new callable_derived<T>(functor))
    {
    }
    function() : base(nullptr) {}
    ~function() { delete base; }
    function(const function &f) : base(f.base->copy()) {}
    function &operator=(const function &f)
    {
        delete base;
        base = f.base->copy();
        return *this;
    }
    Ret operator()(Args... arg) { return (*base)(std::forward<Args>(arg)...); }

  private:
    struct callable_base *base;
};
}  // namespace tinySTL
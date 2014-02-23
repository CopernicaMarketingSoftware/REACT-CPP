/**
 *  Callback.h
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Class definition
 */
template <typename PARAM>
class Callback
{
private:
    /**
     *  The actual function
     *  @var    std::function
     */
    std::function<void(PARAM*)> _func;
    
public:
    /**
     *  The normal callback
     *  @param  func
     */
    Callback(const std::function<void(PARAM *param)> &func) : _func(func) {}
    
    /**
     *  Without a parameter
     *  @param  func
     */
    Callback(const std::function<void()> &func) : _func([func](PARAM *param) { func(); }) {}
    
    /**
     *  Destructor
     */
    virtual ~Callback() {}
    
    /**
     *  Invoke the function
     *  @param  param
     */
    void operator() (PARAM *param)
    {
        _func(param);
    }
};

/**
 *  End namespace
 */
}


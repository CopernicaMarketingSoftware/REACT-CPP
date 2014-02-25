/**
 *  Types.h
 *
 *  Types and callback that are used by the resolver
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Dns {

/**
 *  Types
 */
using IpsResult         =   std::set<IpRecord>;
 
/**
 *  Callbacks
 */
using IpsCallback       =   std::function<void(IpsResult &&ips, const char *error)>;
using IpsSimpleCallback =   std::function<void(IpsResult &&ips)>;


/**
 *  End namespace
 */
}}


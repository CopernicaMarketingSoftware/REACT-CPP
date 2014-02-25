/**
 *  Types.h
 *
 *  Types and callbacks in use by the TCP module
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React { namespace Tcp {

/**
 *  Forward declarations
 */
class Connection;

/**
 *  Callbacks
 */
using ServerCallback    =   std::function<void(const std::shared_ptr<Connection> &connection)>;

/**
 *  End namespace
 */
}}


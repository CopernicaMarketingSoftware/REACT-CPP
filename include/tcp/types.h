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
class Server;

/**
 *  Callbacks
 */
using LostCallback      =   std::function<void()>;
using ConnectCallback   =   std::function<bool(Server *server)>;
using ConstructCallback =   std::function<void(Connection *connection, const char *error)>;
using ConnectedCallback =   std::function<void(const char *error)>;
using DataCallback      =   std::function<bool(const void *buf, size_t size)>;

/**
 *  End namespace
 */
}}


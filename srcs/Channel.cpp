#include "irc.hpp"

vecClient	&Channel::getClientsVec(){
	return (_usersJoin);
}

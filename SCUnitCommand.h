// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitCommand_H_
#define SCUnitCommand_H_

namespace SC {
namespace UnitCommand {


namespace CommandId
{
	enum CommandId
	{
		None, 
		Move, 
		Attack, 
	};
}

/** @brief An abstract class that describes unit orders
 *  @detail
 *  This class describes the constant information about the commands that user orders.
 *  For details, see Command.
 *  @sa Command
 */
class CommandInfo
{
public:
	CommandInfo(CommandId_t cmdid)
		: m_cmdid(cmdid)
	{
	}
	~CommandInfo()
	{
	}
	
	CommandId_t getCommandID() const { return this->m_cmdid; }
	
private:
	CommandId_t m_cmdid;
	
	/** @brief The requirements in order to execute this command
	 */
	struct ms_requirements
	{
		int hit_points;
		int shield;
		int energy;
		int minerals;
		int vespene_gas;
		int supplies;
		std::vector<ObjectId_t> objects;
		//std::vector<UpgradeId_t> upgrades;
	} m_requirements;
	/** @brief A list of orders that will be executed sequentially.
	 */
	//std::vector<UnitAction> m_actionlist;
};

/** @brief An abstract class that describes unit orders
 *  @detail
 *  This class describes commands that user orders.
 *  Basically a Command consists of a list of actions, command parameters, and 
 *  required resources, upgrades, buildings, etc.
 *  Constant information is stored in CommandInfo class.
 *  Once a command is assigned to an object, 
 *  firstly the object checks if the conditions complies with the requirements.
 *  Then the actions specified in m_actionlist will be sequentially executed.
 *  When the final action is finished, the whole command is finished and, 
 *  SC::Object will execute next command in the queue.
 *  @sa CommandInfo
 */
class Command
{
public:
	Command()
	{
	}
	Command(CommandId_t cmdid)
		: m_cmdid(cmdid)
	{
	}
	~Command()
	{
	}
	
	CommandId_t getCommandID() const { return this->m_cmdid; }
	
private:
	CommandId_t m_cmdid;
	const CommandInfo *m_info;
};

class TargetedCommand: public Command
{
public:

private:
	
	struct
	{
		int type; // coordinate or object
		Coordinate coord;
		ObjectPtr obj;
	} m_target;
};


} /* END OF namespace UnitCommand */
} /* END OF namespace SC */

#endif


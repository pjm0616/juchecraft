#ifndef SCUnitCommand_H_
#define SCUnitCommand_H_

namespace SC {


namespace UnitCommandId
{
	enum UnitCommandId
	{
		None, 
		Move, 
		Attack, 
	};
}

class UnitCommand
{
public:
	UnitCommand(UnitCommandId_t cmdid)
		:m_cmdid(cmdid)
	{
	}
	UnitCommand(const UnitCommand &o)
		:m_cmdid(o.m_cmdid)
	{
	}
	~UnitCommand()
	{
	}
	
	UnitCommandId_t getCommandID() const { return this->m_cmdid; }
	
private:
	UnitCommandId_t m_cmdid;
};


} /* END OF namespace SC */

#endif


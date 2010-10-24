#ifndef SCObjectPrototypes_H_
#define SCObjectPrototypes_H_

namespace SC {


/** @brief Manages object prototypes
 */
class ObjectPrototypes
{
public:
	ObjectPrototypes(Game *game);
	~ObjectPrototypes();
	
	bool load(const char *listfile);
	
	// pointer returned by this functions become invalid when this object destructs.
	const ObjectSPtr_t &findByObjectId(ObjectId_t id) { return this->m_obj_protos_by_id[id]; }
	
	// returns a new object based on object `id'
	ObjectSPtr_t newObjectByObjectId(ObjectId_t id) { return ObjectSPtr_t(this->m_obj_protos_by_id[id].get()->duplicate()); }
	
protected:
	ObjectSList m_obj_prototypes;
	ObjectSMap m_obj_protos_by_id;

private:
	void parseObjectData(Object *obj, int stack_idx);
	
	Game *m_game;
	luacpp m_lua;
};


}

#endif


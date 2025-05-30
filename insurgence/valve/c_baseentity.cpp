#include "c_baseentity.h"

#include "vector.h"
#include "datamap.h"

const char* C_BaseEntity::GetClassName()
{
	return this->GetPredDescMap()->dataClassName;
}

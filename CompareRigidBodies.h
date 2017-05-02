#if !defined (COMPARE_RIGID_BODIES_H)
#define COMPARE_RIGID_BODIES_H
#include "CSC2110/Comparator.h"
#include "RigidBody.h"
class CompareRigidBodies : public Comparator<RigidBody, std::string>{
   private:

   public:
      CompareRigidBodies();
      virtual ~CompareRigidBodies();

      int compare(RigidBody* item_1, RigidBody* item_2);
	  int compare(std::string* key, RigidBody* item);
};
CompareRigidBodies::CompareRigidBodies(){}
CompareRigidBodies::~CompareRigidBodies(){}
int CompareRigidBodies::compare(RigidBody* item_1, RigidBody* item_2){
	return compare(item_1->getKey(), item_2);
}
int CompareRigidBodies::compare(std::string* key, RigidBody* item){
	return key->compare(*(item->getKey()));
}

#endif

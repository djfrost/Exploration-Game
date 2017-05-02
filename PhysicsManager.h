#if !defined PHYSICS_MANAGER
#define PHYSICS_MANAGER

#include "btBulletDynamicsCommon.h"
#include "CSC2110/TableAVL.h"

class RigidBody;
class CompoundShape;
class RenderManager;
struct SceneNodeMotion;

class PhysicsManager{
	private:
		RenderManager* renderManager;
		TableAVL<RigidBody, std::string>* rigidBodies;
		TableAVL<CompoundShape, std::string>* compoundShapes;
		btDbvtBroadphase* overlappingPairCache;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btDiscreteDynamicsWorld* dynamicsWorld;
		btSequentialImpulseConstraintSolver* solver;
		void init();
	public:
		void stepPhysicsSimulation(float timeInc);
		void setGravity(float* g);
		void getGravity(float* g);
		void getLinearVelocity(std::string rigidBodyName, float* v);
		void setRigidBodyVelocity(std::string rigidBodyName, float endTime, float* a);
		void applyTorqueImpulse(std::string rigidBodyName, float pitch, float yaw, float roll);
		PhysicsManager(RenderManager* rm);
		~PhysicsManager();
		void createRigidBodies();
		void createCollisionShape(std::string& compoundShapeId, std::string& collisionShape, float mass, float* trans, float* rot,float* collisionShapeParams);
		void updateRigidBodies();
		void setDebugDrawer(bool debugMode);
		void updateRigidBodyVelocity(float timeInc);
		void applyImpulse(std::string& rigidBodyName, float ns, float ew,float other);
		//btRigidBody* LoadMeshFromObj(std::string collisionFile, float* pos, float* scale, float* rot);
};

#endif
